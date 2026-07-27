module top (
    input C,
    input signed [15:0] A, B,
    input S,
    output reg signed [15:0] Q, R,
    output reg F
);

    reg signed [15:0] dividend, divisor, quotient;
    reg sign_q, sign_r;
    integer i;

    always @(posedge C) begin
        if (S) begin
            // Handle signs
            sign_q = A[15] ^ B[15];     // Quotient sign
            sign_r = A[15];             // Remainder sign

            // Get absolute values
            dividend = A[15] ? -A : A;
            divisor  = B[15] ? -B : B;
            quotient = 0;

            // Repeated subtraction
            for (i = 0; i < 32767; i = i + 1)
                if (dividend >= divisor) begin
                    dividend = dividend - divisor;
                    quotient = quotient + 1;
                end else begin
                    i = 32767; // exit early
                end

            // Apply signs
            Q <= sign_q ? -quotient : quotient;
            R <= sign_r ? -dividend : dividend;
            F <= 1;
        end else begin
            F <= 0;
        end
    end

endmodule