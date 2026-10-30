`timescale 1ns / 1ps

module main (
    input  wire        clk,
    input  wire        reset,
    input  wire [11:0] sw,

    output wire [3:0] vgaRed,
    output wire [3:0] vgaGreen,
    output wire [3:0] vgaBlue,
    output wire       HSync,
    output wire       VSync
);

    wire pixel_clk;
    wire locked;
    wire video_on;

    wire system_reset;


    // Keep the rest of the system in reset until
    // the MMCM has locked onto the requested clock.
    assign system_reset = reset | ~locked;


    // 100 MHz -> 25.18293 MHz
    clock_generator clk_gen (
        .clk_100mhz(clk),
        .reset(reset),
        .pixel_clk(pixel_clk),
        .locked(locked)
    );


    // VGA horizontal / vertical timing
    vga_timing timing (
        .pixel_clk(pixel_clk),
        .reset(system_reset),
        .HSync(HSync),
        .VSync(VSync),
        .video_on(video_on)
    );


    // Color generation
    pixel_writer writer (
        .switches(sw),
        .video_on(video_on),
        .reset(system_reset),
        .vgaRed(vgaRed),
        .vgaGreen(vgaGreen),
        .vgaBlue(vgaBlue)
    );

endmodule