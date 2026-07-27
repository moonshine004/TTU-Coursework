// xor_op.v - XOR operation
module xor_op(
    input [7:0] a,
    input [7:0] b,
    output [7:0] result
);
    assign result = a ^ b;
endmodule
