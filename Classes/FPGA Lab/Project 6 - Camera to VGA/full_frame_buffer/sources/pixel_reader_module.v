`timescale 1ns / 1ps

module pixel_reader_module (
    input  wire        pixel_clk,
    input  wire        reset,

    input  wire [9:0]  pixel_x,
    input  wire [9:0]  pixel_y,
    input  wire        video_on,

    // ---------------------------------------------------------
    // SCALE CONTROL
    //
    // scale_1x = 0:
    //     320x240 -> 640x480
    //
    // scale_1x = 1:
    //     native 320x240 image centered on screen
    // ---------------------------------------------------------
    input  wire        scale_1x,

    input  wire [15:0] bram_pixel,

    output reg  [16:0] bram_address,

    output reg  [3:0]  vgaRed,
    output reg  [3:0]  vgaGreen,
    output reg  [3:0]  vgaBlue
);

    // =========================================================
    // CAMERA PIXEL COORDINATES
    // =========================================================

    reg [8:0] camera_x;
    reg [7:0] camera_y;

    reg image_active;


    // =========================================================
    // DISPLAY MODE SELECTION
    // =========================================================
    //
    // VGA display:
    //
    //     640 x 480
    //
    // Stored camera frame:
    //
    //     320 x 240
    //
    //
    // 2x MODE:
    //
    // Each camera pixel becomes a 2x2 VGA block.
    //
    //
    // 1x MODE:
    //
    // Camera image is centered.
    //
    // Horizontal offset:
    //
    //     (640 - 320) / 2 = 160
    //
    // Vertical offset:
    //
    //     (480 - 240) / 2 = 120
    //
    // Therefore native image occupies:
    //
    //     X = 160 through 479
    //     Y = 120 through 359
    //
    // =========================================================

    always @(*) begin

        camera_x    = 9'd0;
        camera_y    = 8'd0;
        image_active = 1'b0;


        if (video_on) begin

            // -------------------------------------------------
            // 1x NATIVE SCALE
            // -------------------------------------------------

            if (scale_1x) begin

                if (
                    (pixel_x >= 10'd160) &&
                    (pixel_x <  10'd480) &&
                    (pixel_y >= 10'd120) &&
                    (pixel_y <  10'd360)
                ) begin

                    camera_x =
                        pixel_x - 10'd160;

                    camera_y =
                        pixel_y - 10'd120;

                    image_active = 1'b1;

                end

            end


            // -------------------------------------------------
            // 2x SCALE
            // -------------------------------------------------

            else begin

                // Divide VGA coordinates by two.

                camera_x =
                    pixel_x[9:1];

                camera_y =
                    pixel_y[8:1];

                image_active = 1'b1;

            end

        end

    end


    // =========================================================
    // FRAME BUFFER ADDRESS
    // =========================================================
    //
    // Address:
    //
    //     address = y * 320 + x
    //
    // Since:
    //
    //     320 = 256 + 64
    //
    // we can calculate:
    //
    //     y * 320
    //
    // as:
    //
    //     (y << 8) + (y << 6)
    //
    // =========================================================

    wire [16:0] camera_y_ext;

    assign camera_y_ext = {
        9'b000000000,
        camera_y
    };


    always @(*) begin

        if (image_active) begin

            bram_address =
                (camera_y_ext << 8)
                +
                (camera_y_ext << 6)
                +
                camera_x;

        end

        else begin

            bram_address = 17'd0;

        end

    end


    // =========================================================
    // BRAM READ LATENCY
    // =========================================================
    //
    // The BRAM read is synchronous.
    //
    // The requested address is presented during one pixel clock
    // and the corresponding RGB565 data appears one clock later.
    //
    // Therefore image_active must also be delayed one clock.
    // =========================================================

    reg image_active_delayed;

    always @(posedge pixel_clk or posedge reset) begin

        if (reset) begin

            image_active_delayed <= 1'b0;

        end

        else begin

            image_active_delayed <= image_active;

        end

    end


    // =========================================================
    // RGB565 -> RGB444
    // =========================================================
    //
    // Stored RGB565:
    //
    // [15:11] = Red   (5 bits)
    // [10:5]  = Green (6 bits)
    // [4:0]   = Blue  (5 bits)
    //
    // Basys 3 VGA:
    //
    // Red   = 4 bits
    // Green = 4 bits
    // Blue  = 4 bits
    //
    // Keep the most significant bits.
    // =========================================================

    always @(*) begin

        if (image_active_delayed) begin

            vgaRed =
                bram_pixel[15:12];

            vgaGreen =
                bram_pixel[10:7];

            vgaBlue =
                bram_pixel[4:1];

        end

        else begin

            // Black background around the native-size image.

            vgaRed   = 4'b0000;
            vgaGreen = 4'b0000;
            vgaBlue  = 4'b0000;

        end

    end

endmodule