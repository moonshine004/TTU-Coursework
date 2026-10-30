`timescale 1ns / 1ps

module frame_buffer_module (
    // Camera write side
    input  wire        write_clk,
    input  wire        write_enable,
    input  wire [16:0] write_address,
    input  wire [15:0] write_data,

    // VGA read side
    input  wire        read_clk,
    input  wire [16:0] read_address,

    output reg  [15:0] read_data
);

    localparam integer DEPTH = 76800;


    (* ram_style = "block" *)
    reg [15:0] memory [0:DEPTH-1];


    // ---------------------------------------------------------
    // CAMERA WRITE PORT
    // ---------------------------------------------------------

    always @(posedge write_clk) begin

        if (
            write_enable &&
            write_address < DEPTH
        ) begin

            memory[write_address] <=
                write_data;

        end

    end


    // ---------------------------------------------------------
    // VGA READ PORT
    // ---------------------------------------------------------

    always @(posedge read_clk) begin

        if (read_address < DEPTH) begin

            read_data <=
                memory[read_address];

        end

        else begin

            read_data <= 16'h0000;

        end

    end

endmodule