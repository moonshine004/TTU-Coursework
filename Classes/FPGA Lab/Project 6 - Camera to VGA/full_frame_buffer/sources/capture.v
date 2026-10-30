`timescale 1ns / 1ps

module capture (
    input  wire        reset,
    input  wire        config_done,

    input  wire        cam_pclk,
    input  wire        cam_href,
    input  wire        cam_vsync,
    input  wire [7:0]  cam_data,

    output reg  [15:0] pixel_data,
    output reg  [16:0] write_address,
    output reg         write_enable,

    output reg         frame_done,
    output reg         frame_seen
);

    localparam integer FRAME_PIXELS = 76800;


    // ---------------------------------------------------------
    // CONFIG_DONE CLOCK-DOMAIN CROSSING
    // ---------------------------------------------------------

    reg config_meta;
    reg config_sync;

    always @(posedge cam_pclk or posedge reset) begin

        if (reset) begin

            config_meta <= 1'b0;
            config_sync <= 1'b0;

        end

        else begin

            config_meta <= config_done;
            config_sync <= config_meta;

        end

    end


    // ---------------------------------------------------------
    // CAMERA DATA CAPTURE
    // ---------------------------------------------------------

    reg [7:0] first_byte;
    reg       byte_select;

    reg [16:0] address_counter;

    reg previous_vsync;


    always @(posedge cam_pclk or posedge reset) begin

        if (reset) begin

            first_byte <= 8'h00;
            byte_select <= 1'b0;

            address_counter <= 17'd0;

            pixel_data <= 16'h0000;

            write_address <= 17'd0;
            write_enable <= 1'b0;

            previous_vsync <= 1'b0;

            frame_done <= 1'b0;
            frame_seen <= 1'b0;

        end

        else begin

            write_enable <= 1'b0;
            frame_done <= 1'b0;

            previous_vsync <= cam_vsync;


            if (!config_sync) begin

                address_counter <= 17'd0;
                byte_select <= 1'b0;

            end


            else begin

                // ---------------------------------------------
                // FRAME BOUNDARY
                // ---------------------------------------------

                if (
                    cam_vsync &&
                    !previous_vsync
                ) begin

                    if (address_counter != 0) begin

                        frame_done <= 1'b1;
                        frame_seen <= 1'b1;

                    end

                    address_counter <= 17'd0;
                    byte_select <= 1'b0;

                end


                // ---------------------------------------------
                // VALID LINE DATA
                // ---------------------------------------------

                else if (
                    cam_href &&
                    !cam_vsync
                ) begin

                    // First byte
                    if (byte_select == 1'b0) begin

                        first_byte <= cam_data;

                        byte_select <= 1'b1;

                    end


                    // Second byte
                    else begin

                        pixel_data <= {
                            first_byte,
                            cam_data
                        };

                        if (
                            address_counter <
                            FRAME_PIXELS
                        ) begin

                            write_address <=
                                address_counter;

                            write_enable <= 1'b1;

                            address_counter <=
                                address_counter + 1'b1;

                        end

                        byte_select <= 1'b0;

                    end

                end


                // ---------------------------------------------
                // OUTSIDE ACTIVE LINE
                // ---------------------------------------------

                else begin

                    byte_select <= 1'b0;

                end

            end

        end

    end

endmodule