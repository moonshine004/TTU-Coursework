// invert.v - INVERT operation
module invert(
    input [7:0] a,
    input [7:0] b,  // Not used, but included for consistent interface
    output [7:0] result
);
    assign result = ~a;
endmodule
