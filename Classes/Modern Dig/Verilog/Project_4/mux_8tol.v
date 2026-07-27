// mux_8to1.v - 8-to-1 Multiplexer
module mux_8to1(
    input [7:0] in0,  // Addition
    input [7:0] in1,  // Subtraction
    input [7:0] in2,  // AND
    input [7:0] in3,  // OR
    input [7:0] in4,  // XOR
    input [7:0] in5,  // INVERT
    input [7:0] in6,  // Shift Left
    input [7:0] in7,  // Shift Right
    input [2:0] select,
    output reg [7:0] out
);
    parameter ADD_OP      = 3'b000;
    parameter SUBTRACT_OP = 3'b001;
    parameter AND_OP      = 3'b010;
    parameter OR_OP       = 3'b011;
    parameter XOR_OP      = 3'b100;
    parameter INVERT_OP   = 3'b101;
    parameter SHIFT_L_OP  = 3'b110;
    parameter SHIFT_R_OP  = 3'b111; 


    always @(in0 or in1 or in2 or in3 or in4 or in5 or in6 or in7 or select) begin
        case(select)
            ADD_OP: out = in0;  // Addition
            SUBTRACT_OP: out = in1;  // Subtraction
            AND_OP: out = in2;  // AND
            OR_OP: out = in3;  // OR
            XOR_OP: out = in4;  // XOR
            INVERT_OP: out = in5;  // INVERT
            SHIFT_L_OP: out = in6;  // Shift Left
            SHIFT_R_OP: out = in7;  // Shift Right
            default: out = 8'b0;
        endcase
    end
endmodule
