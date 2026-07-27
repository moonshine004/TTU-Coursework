// or_op.v - OR operation
module or_op(
    input [7:0] a,
    input [7:0] b,
    output [7:0] result
);
    assign result = a | b;
endmodule
