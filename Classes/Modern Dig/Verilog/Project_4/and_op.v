// and_op.v - AND operation
module and_op(
    input [7:0] a,
    input [7:0] b,
    output [7:0] result
);
    assign result = a & b;
endmodule
