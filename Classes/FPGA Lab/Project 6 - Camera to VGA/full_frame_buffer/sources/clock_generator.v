`timescale 1ns / 1ps

module clock_generator (
    input  wire clk_100mhz,
    input  wire reset,

    output wire pixel_clk,
    output wire cam_xclk,
    output wire locked
);

    clk_wiz_0 clock_wizard (
        .clk_in1(clk_100mhz),
        .reset(reset),

        .pixel_clk(pixel_clk),
        .cam_xclk(cam_xclk),

        .locked(locked)
    );

endmodule