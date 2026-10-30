`timescale 1ns / 1ps

module top (
    // =========================================================
    // BASYS 3
    // =========================================================
    input  wire        clk,
    input  wire [15:0] sw,
    input  wire        btnC,
    input  wire        btnL,

    output wire [15:0] led,

    // =========================================================
    // OV7670
    // =========================================================
    input  wire        cam_pclk,
    input  wire        cam_vsync,
    input  wire        cam_href,
    input  wire [7:0]  cam_d,

    output wire        cam_xclk,
    output wire        cam_sioc,
    inout  wire        cam_siod,
    output wire        cam_rst_n,
    output wire        cam_pwdn,

    // =========================================================
    // VGA
    // =========================================================
    output wire [3:0]  vgaRed,
    output wire [3:0]  vgaGreen,
    output wire [3:0]  vgaBlue,

    output reg         Hsync,
    output reg         Vsync
);

    // =========================================================
    // CLOCK GENERATOR
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

    assign system_reset =
        btnC | ~clock_locked;


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
    wire        camera_pixel_valid;

    wire frame_seen;

    capture capture_inst (
        .reset(system_reset),
        .config_done(config_done),

        .cam_pclk(cam_pclk),
        .cam_href(cam_href),
        .cam_vsync(cam_vsync),
        .cam_data(cam_d),

        .pixel_data(camera_pixel),
        .pixel_valid(camera_pixel_valid),

        .frame_seen(frame_seen)
    );


    // =========================================================
    // ASYNCHRONOUS MULTI-ROW FIFO
    // =========================================================

    wire [15:0] fifo_pixel;

    wire fifo_full;
    wire fifo_empty;

    wire fifo_overflow;
    wire fifo_underflow;

    wire fifo_wr_rst_busy;
    wire fifo_rd_rst_busy;

    wire [16:0] fifo_rd_count;
    wire [16:0] fifo_wr_count;

    wire fifo_rd_en;

    wire fifo_wr_en;

    assign fifo_wr_en =
        camera_pixel_valid &&
        !fifo_full &&
        !fifo_wr_rst_busy;


    row_fifo row_buffer (
        .reset(system_reset),

        // Camera domain
        .write_clk(cam_pclk),
        .write_enable(fifo_wr_en),
        .write_data(camera_pixel),

        .full(fifo_full),
        .overflow(fifo_overflow),
        .write_reset_busy(fifo_wr_rst_busy),
        .write_count(fifo_wr_count),

        // VGA domain
        .read_clk(pixel_clk),
        .read_enable(fifo_rd_en),
        .read_data(fifo_pixel),

        .empty(fifo_empty),
        .underflow(fifo_underflow),
        .read_reset_busy(fifo_rd_rst_busy),
        .read_count(fifo_rd_count)
    );


    // =========================================================
    // START VGA ONLY AFTER BUFFER HAS DATA
    //
    // 32768 pixels = ~51 complete rows.
    //
    // Starting half-way into the FIFO gives us room for timing
    // differences in either direction.
    // =========================================================

    localparam [16:0] START_LEVEL =
        17'd32768;

    reg stream_started;

    always @(posedge pixel_clk or posedge system_reset) begin

        if (system_reset) begin

            stream_started <= 1'b0;

        end

        else if (
            !stream_started &&
            !fifo_rd_rst_busy &&
            fifo_rd_count >= START_LEVEL
        ) begin

            stream_started <= 1'b1;

        end

    end


    wire display_reset;

    assign display_reset =
        system_reset | ~stream_started;


    // =========================================================
    // VGA TIMING
    // =========================================================

    wire [9:0] pixel_x;
    wire [9:0] pixel_y;

    wire video_on;

    wire raw_hsync;
    wire raw_vsync;

    vga_timing_module timing (
        .pixel_clk(pixel_clk),
        .reset(display_reset),

        .HSync(raw_hsync),
        .VSync(raw_vsync),

        .video_on(video_on),

        .pixel_x(pixel_x),
        .pixel_y(pixel_y)
    );


    // =========================================================
    // STREAM → VGA
    // =========================================================

    wire stream_fault;

    stream_reader_module reader (
        .pixel_clk(pixel_clk),
        .reset(display_reset),

        .video_on(video_on),

        .fifo_pixel(fifo_pixel),
        .fifo_empty(fifo_empty),

        .fifo_read_enable(fifo_rd_en),

        .stream_fault(stream_fault),

        .vgaRed(vgaRed),
        .vgaGreen(vgaGreen),
        .vgaBlue(vgaBlue)
    );


    // =========================================================
    // ONE-CLOCK VGA PIPELINE DELAY
    // =========================================================

    always @(posedge pixel_clk or posedge display_reset) begin

        if (display_reset) begin

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
    //
    // LED0 = Clock Wizard locked
    // LED1 = Camera configured
    // LED2 = Camera frame boundary seen
    // LED3 = VGA streaming has started
    // LED4 = FIFO full
    // LED5 = FIFO empty
    // LED6 = VGA stream fault / underflow
    // LED7 = XPM FIFO overflow
    // =========================================================

    assign led[0] = clock_locked;
    assign led[1] = config_done;
    assign led[2] = frame_seen;
    assign led[3] = stream_started;

    assign led[4] = fifo_full;
    assign led[5] = fifo_empty;
    assign led[6] = stream_fault;
    assign led[7] = fifo_overflow;

    assign led[15:8] = 8'b0;

endmodule