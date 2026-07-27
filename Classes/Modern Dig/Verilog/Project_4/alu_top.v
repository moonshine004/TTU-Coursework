`include "add.v"
`include "subtract.v"
`include "and_op.v"
`include "invert.v"
`include "mux_8tol.v"
`include "or_op.v"
`include "shift_left.v"
`include "shift_right.v"
`include "xor_op.v"
`include "alu_constants.v"

// alu_top.v - Top-level ALU module
module alu_top(
    input [7:0] opA,
    input [7:0] opB,
    input [2:0] opS,
    output [7:0] Result
);
    // Wires to connect modules
    wire [7:0] add_result;
    wire [7:0] sub_result;
    wire [7:0] and_result;
    wire [7:0] or_result;
    wire [7:0] xor_result;
    wire [7:0] invert_result;
    wire [7:0] shift_left_result;
    wire [7:0] shift_right_result;
    
    // Instantiate all operation modules
    add add_op (
        .a(opA),
        .b(opB),
        .result(add_result)
    );
    
    subtract sub_op (
        .a(opA),
        .b(opB),
        .result(sub_result)
    );
    
    and_op and_operation (
        .a(opA),
        .b(opB),
        .result(and_result)
    );
    
    or_op or_operation (
        .a(opA),
        .b(opB),
        .result(or_result)
    );
    
    xor_op xor_operation (
        .a(opA),
        .b(opB),
        .result(xor_result)
    );
    
    invert invert_op (
        .a(opA),
        .b(opB),  // Not used
        .result(invert_result)
    );
    
    shift_left shift_left_op (
        .a(opA),
        .b(opB),
        .result(shift_left_result)
    );
    
    shift_right shift_right_op (
        .a(opA),
        .b(opB),
        .result(shift_right_result)
    );
    
    // Instantiate multiplexer to select the output
    mux_8to1 result_mux (
        .in0(add_result),
        .in1(sub_result),
        .in2(and_result),
        .in3(or_result),
        .in4(xor_result),
        .in5(invert_result),
        .in6(shift_left_result),
        .in7(shift_right_result),
        .select(opS),
        .out(Result)
    );
endmodule
