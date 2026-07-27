// shift_left.v - Shift Left operation
module shift_left(
    input [7:0] a,
    input [7:0] b,  // b[2:0] determines shift amount
    output [7:0] result
);
    assign result = a << b[2:0];
endmodule
