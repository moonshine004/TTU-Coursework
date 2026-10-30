`timescale 1ns / 1ps

module pixel_writer (
    input  wire [11:0] switches,
    input  wire        video_on,
    input  wire        reset,

    output reg [3:0] vgaRed,
    output reg [3:0] vgaGreen,
    output reg [3:0] vgaBlue
);

    always @(*) begin

        if (reset) begin
            vgaRed   = 4'b0000;
            vgaGreen = 4'b0000;
            vgaBlue  = 4'b0000;
        end

        else if (video_on) begin
            vgaRed   = switches[11:8];
            vgaGreen = switches[7:4];
            vgaBlue  = switches[3:0];
        end

        else begin
            vgaRed   = 4'b0000;
            vgaGreen = 4'b0000;
            vgaBlue  = 4'b0000;
        end

    end

endmodule