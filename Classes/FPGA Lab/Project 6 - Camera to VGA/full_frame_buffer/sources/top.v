`timescale 1ns / 1ps

module top (
    // ---------------------------------------------------------
    // BASYS 3
    // ---------------------------------------------------------
    input  wire        clk,
    input  wire [15:0] sw,
    input  wire        btnC,
    input  wire        btnL,

    output wire [15:0] led,

    // ---------------------------------------------------------
    // OV7670
    // ---------------------------------------------------------
    input  wire        cam_pclk,
    input  wire        cam_vsync,
    input  wire        cam_href,
    input  wire [7:0]  cam_d,

    output wire        cam_xclk,
    output wire        cam_sioc,
    inout  wire        cam_siod,
    output wire        cam_rst_n,
    output wire        cam_pwdn,

    // ---------------------------------------------------------
    // VGA
    // ---------------------------------------------------------
    output wire [3:0]  vgaRed,
    output wire [3:0]  vgaGreen,
    output wire [3:0]  vgaBlue,
    output reg         Hsync,
    output reg         Vsync
);

    // =========================================================
    // CLOCK GENERATION
    // =========================================================

    wire pixel_clk;
    wire clock_locked;

    clock_generator clocks (
        .clk_100mhz(clk),
        .reset(btnC),

        .pixel_clk(pixel_clk),
        .cam_xclk(cam_xclk),

        .locked(clock_locked)
    );

    wire system_reset;

    assign system_reset = btnC | ~clock_locked;


    // =========================================================
    // CAMERA CONFIGURATION
    // =========================================================

    wire config_done;

    camera_config camera_cfg (
        .clk(clk),
        .reset(system_reset),

        .cam_sioc(cam_sioc),
        .cam_siod(cam_siod),

        .cam_rst_n(cam_rst_n),
        .cam_pwdn(cam_pwdn),

        .config_done(config_done)
    );


    // =========================================================
    // CAMERA CAPTURE
    // =========================================================

    wire [15:0] camera_pixel;
    wire [16:0] camera_write_address;
    wire        camera_write_enable;

    wire frame_done;
    wire frame_seen;

    capture capture (
        .reset(system_reset),
        .config_done(config_done),

        .cam_pclk(cam_pclk),
        .cam_href(cam_href),
        .cam_vsync(cam_vsync),
        .cam_data(cam_d),

        .pixel_data(camera_pixel),
        .write_address(camera_write_address),
        .write_enable(camera_write_enable),

        .frame_done(frame_done),
        .frame_seen(frame_seen)
    );


    // =========================================================
    // VGA TIMING
    // =========================================================

    wire [9:0] pixel_x;
    wire [9:0] pixel_y;
    wire       video_on;

    wire raw_hsync;
    wire raw_vsync;

    vga_timing_module timing (
        .pixel_clk(pixel_clk),
        .reset(system_reset),

        .HSync(raw_hsync),
        .VSync(raw_vsync),

        .video_on(video_on),

        .pixel_x(pixel_x),
        .pixel_y(pixel_y)
    );


    // =========================================================
    // FRAME BUFFER
    // =========================================================

    wire [16:0] display_read_address;
    wire [15:0] display_pixel;

    frame_buffer_module framebuffer (
        // Camera write side
        .write_clk(cam_pclk),
        .write_enable(camera_write_enable),
        .write_address(camera_write_address),
        .write_data(camera_pixel),

        // VGA read side
        .read_clk(pixel_clk),
        .read_address(display_read_address),
        .read_data(display_pixel)
    );


    // =========================================================
    // VGA PIXEL READER
    //
    // SW0:
    //
    // 0 = 2x scale
    //     320x240 camera image fills 640x480 display
    //
    // 1 = 1x native scale
    //     320x240 image centered on 640x480 display
    // =========================================================

    pixel_reader_module reader (
        .pixel_clk(pixel_clk),
        .reset(system_reset),

        .pixel_x(pixel_x),
        .pixel_y(pixel_y),
        .video_on(video_on),

        .scale_1x(sw[0]),

        .bram_pixel(display_pixel),
        .bram_address(display_read_address),

        .vgaRed(vgaRed),
        .vgaGreen(vgaGreen),
        .vgaBlue(vgaBlue)
    );


    // =========================================================
    // SYNC DELAY
    //
    // BRAM read has one pixel-clock latency.
    // Delay HSync/VSync one clock to match the pixel data.
    // =========================================================

    always @(posedge pixel_clk or posedge system_reset) begin

        if (system_reset) begin

            Hsync <= 1'b1;
            Vsync <= 1'b1;

        end

        else begin

            Hsync <= raw_hsync;
            Vsync <= raw_vsync;

        end

    end


    // =========================================================
    // DEBUG LEDs
    // =========================================================

    assign led[0] = clock_locked;
    assign led[1] = config_done;
    assign led[2] = frame_seen;

    assign led[15:3] = 13'b0;

endmodule