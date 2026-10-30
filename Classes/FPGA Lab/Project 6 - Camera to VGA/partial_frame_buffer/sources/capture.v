`timescale 1ns / 1ps

module capture (
    input  wire        reset,
    input  wire        config_done,

    input  wire        cam_pclk,
    input  wire        cam_href,
    input  wire        cam_vsync,
    input  wire [7:0]  cam_data,

    output reg  [15:0] pixel_data,
    output reg         pixel_valid,

    output reg         frame_seen
);

    // =========================================================
    // SYNCHRONIZE CONFIG_DONE INTO PCLK DOMAIN
    // =========================================================

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


    // =========================================================
    // CAMERA STATE
    // =========================================================

    reg previous_vsync;
    reg previous_href;

    reg frame_active;

    reg byte_select;

    reg [7:0] first_byte;

    reg [9:0] pixel_x;
    reg [8:0] pixel_y;


    always @(posedge cam_pclk or posedge reset) begin

        if (reset) begin

            previous_vsync <= 1'b0;
            previous_href  <= 1'b0;

            frame_active <= 1'b0;

            byte_select <= 1'b0;
            first_byte  <= 8'd0;

            pixel_x <= 10'd0;
            pixel_y <= 9'd0;

            pixel_data  <= 16'd0;
            pixel_valid <= 1'b0;

            frame_seen <= 1'b0;

        end

        else begin

            pixel_valid <= 1'b0;

            previous_vsync <= cam_vsync;
            previous_href  <= cam_href;


            // -------------------------------------------------
            // CAMERA NOT CONFIGURED
            // -------------------------------------------------

            if (!config_sync) begin

                frame_active <= 1'b0;

                byte_select <= 1'b0;

                pixel_x <= 10'd0;
                pixel_y <= 9'd0;

            end


            else begin

                // ---------------------------------------------
                // NEW FRAME
                // ---------------------------------------------

                if (
                    cam_vsync &&
                    !previous_vsync
                ) begin

                    frame_active <= 1'b1;

                    pixel_x <= 10'd0;
                    pixel_y <= 9'd0;

                    byte_select <= 1'b0;

                    frame_seen <= 1'b1;

                end


                // ---------------------------------------------
                // START OF ACTIVE CAMERA ROW
                // ---------------------------------------------

                if (
                    cam_href &&
                    !previous_href &&
                    !cam_vsync
                ) begin

                    pixel_x <= 10'd0;
                    byte_select <= 1'b0;

                end


                // ---------------------------------------------
                // ACTIVE IMAGE DATA
                // ---------------------------------------------

                if (
                    frame_active &&
                    cam_href &&
                    !cam_vsync &&
                    pixel_y < 9'd480
                ) begin

                    // First byte of RGB565 pixel
                    if (!byte_select) begin

                        first_byte <= cam_data;

                        byte_select <= 1'b1;

                    end


                    // Second byte
                    else begin

                        if (pixel_x < 10'd640) begin

                            pixel_data <= {
                                first_byte,
                                cam_data
                            };

                            pixel_valid <= 1'b1;

                            pixel_x <=
                                pixel_x + 1'b1;

                        end

                        byte_select <= 1'b0;

                    end

                end


                // ---------------------------------------------
                // END OF ROW
                // ---------------------------------------------

                if (
                    !cam_href &&
                    previous_href
                ) begin

                    byte_select <= 1'b0;

                    if (
                        frame_active &&
                        pixel_y < 9'd479
                    ) begin

                        pixel_y <=
                            pixel_y + 1'b1;

                    end

                end

            end

        end

    end

endmodule