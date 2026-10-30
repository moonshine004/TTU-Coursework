`timescale 1ns / 1ps

module clock (
    input  wire clk_100mhz,
    input  wire reset,
    output wire pixel_clk,
    output wire locked
);

    clk_wiz_0 clk_wiz_inst (
        .clk_in1  (clk_100mhz),
        .reset    (reset),
        .clk_out1 (pixel_clk),
        .locked   (locked)
    );

endmodule