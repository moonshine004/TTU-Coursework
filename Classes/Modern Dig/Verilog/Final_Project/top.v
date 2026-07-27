// don't submit this cause johnston said to not use / or % (insert sad face)
// Author   : Amna Malik
// Project  : 16-bit Signed Divider (Verilog)
// Course   : Modern Digital Systems (Final Project)
// Date     : [05/09/2025]

module top (
    input C,                        // Clock
    input signed [15:0] A, B,       // signed dividend and divisor
    input S,                        // signal
    output reg signed [15:0] Q, R,  // signed quotient and remainder
    output reg F                    // done flag
);

    always @(posedge C) begin       // rising clock edge, start if signal if high
        if (S) begin
            Q <= A / B;             // quotient
            R <= A % B;             // remainder
            F <= 1;                 // congrats it works! 
        end else begin
            F <= 0;                 // something went wrong, cry about it + fail
        end
    end

endmodule