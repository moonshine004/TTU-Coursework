module top (
    input C,                        // clock
    input signed [15:0] A, B,       // dividend and divisor
    input S,                        // signal
    output reg signed [15:0] Q, R,  // quotient and remainder
    output reg F                    // flag
);

    reg signed [15:0] dividend;     // AV for A
    reg signed [15:0] divisor;      // AV for B
    reg signed [15:0] quotient;     // subtraction
    reg sign_q, sign_r;             // final sign
    integer i;                      // loop
    reg done;                       // exit loop early

    always @(posedge C) begin
        if (S) begin
            // Track signs
            sign_q = A[15] ^ B[15];   // Quotient sign = A xor B
            sign_r = A[15];           // Remainder sign = same as A

            // Absolute values
            dividend = A[15] ? -A : A;  
            divisor  = B[15] ? -B : B;
            quotient = 0;
            done = 0;

            // Subtract divisor until what's left is less than divisor
            for (i = 0; i < 32767 && !done; i = i + 1) begin
                if (dividend >= divisor) begin
                    dividend = dividend - divisor;
                    quotient = quotient + 1;
                end else begin
                    done = 1;
                end
            end

            // Apply signs back
            Q <= sign_q ? -quotient : quotient;
            R <= sign_r ? -dividend : dividend;
            F <= 1;                                // yay you did it!
        end else begin
            F <= 0;                                // you're dumb, try again sike you fail
        end
    end

endmodule