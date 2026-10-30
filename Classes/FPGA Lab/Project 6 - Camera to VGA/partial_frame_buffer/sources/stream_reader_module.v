`timescale 1ns / 1ps

module stream_reader_module (
    input  wire        pixel_clk,
    input  wire        reset,

    input  wire        video_on,

    input  wire [15:0] fifo_pixel,
    input  wire        fifo_empty,

    output reg         fifo_read_enable,

    output reg         stream_fault,

    output reg  [3:0]  vgaRed,
    output reg  [3:0]  vgaGreen,
    output reg  [3:0]  vgaBlue
);

    reg [15:0] pixel_latched;

    reg pixel_valid_delayed;


    // =========================================================
    // FIFO READER
    // =========================================================

    always @(posedge pixel_clk or posedge reset) begin

        if (reset) begin

            fifo_read_enable <= 1'b0;

            pixel_latched <= 16'd0;

            pixel_valid_delayed <= 1'b0;

            stream_fault <= 1'b0;

        end

        else begin

            fifo_read_enable <= 1'b0;

            pixel_valid_delayed <= 1'b0;


            // Once an underflow happens during visible video,
            // stop using the stream until reset.
            if (!stream_fault) begin

                if (video_on) begin

                    if (!fifo_empty) begin

                        // FWFT means current word is already
                        // available on fifo_pixel.

                        pixel_latched <=
                            fifo_pixel;

                        fifo_read_enable <=
                            1'b1;

                        pixel_valid_delayed <=
                            1'b1;

                    end

                    else begin

                        stream_fault <=
                            1'b1;

                    end

                end

            end

        end

    end


    // =========================================================
    // RGB565 → RGB444
    // =========================================================

    always @(*) begin

        if (
            pixel_valid_delayed &&
            !stream_fault
        ) begin

            vgaRed =
                pixel_latched[15:12];

            vgaGreen =
                pixel_latched[10:7];

            vgaBlue =
                pixel_latched[4:1];

        end

        else begin

            vgaRed   = 4'b0000;
            vgaGreen = 4'b0000;
            vgaBlue  = 4'b0000;

        end

    end

endmodule