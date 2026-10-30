`timescale 1ns / 1ps

module vga_timing_module (
    input  wire       pixel_clk,
    input  wire       reset,

    output reg        HSync,
    output reg        VSync,

    output wire       video_on,

    output wire [9:0] pixel_x,
    output wire [9:0] pixel_y
);

    localparam H_VISIBLE = 640;
    localparam H_FRONT   = 16;
    localparam H_SYNC    = 96;
    localparam H_BACK    = 48;
    localparam H_TOTAL   = 800;

    localparam V_VISIBLE = 480;
    localparam V_FRONT   = 10;
    localparam V_SYNC    = 2;
    localparam V_BACK    = 33;
    localparam V_TOTAL   = 525;


    reg [9:0] h_count;
    reg [9:0] v_count;


    // =========================================================
    // COUNTERS
    // =========================================================

    always @(posedge pixel_clk or posedge reset) begin

        if (reset) begin

            h_count <= 10'd0;
            v_count <= 10'd0;

        end

        else begin

            if (h_count == H_TOTAL - 1) begin

                h_count <= 10'd0;

                if (v_count == V_TOTAL - 1)

                    v_count <= 10'd0;

                else

                    v_count <=
                        v_count + 1'b1;

            end

            else begin

                h_count <=
                    h_count + 1'b1;

            end

        end

    end


    assign pixel_x =
        h_count;

    assign pixel_y =
        v_count;


    // =========================================================
    // HSYNC
    // =========================================================

    always @(*) begin

        if (
            h_count >=
                H_VISIBLE + H_FRONT
            &&
            h_count <
                H_VISIBLE + H_FRONT + H_SYNC
        )

            HSync = 1'b0;

        else

            HSync = 1'b1;

    end


    // =========================================================
    // VSYNC
    // =========================================================

    always @(*) begin

        if (
            v_count >=
                V_VISIBLE + V_FRONT
            &&
            v_count <
                V_VISIBLE + V_FRONT + V_SYNC
        )

            VSync = 1'b0;

        else

            VSync = 1'b1;

    end


    // =========================================================
    // ACTIVE VIDEO
    // =========================================================

    assign video_on =
        (h_count < H_VISIBLE) &&
        (v_count < V_VISIBLE);

endmodule