`include "alex_top.v"

module testbench_basic;

    reg clk;
    reg signed [15:0] A, B;
    reg S;

    wire signed [15:0] Q, R;
    wire F;

    top uut (
        .C(clk),
        .A(A),
        .B(B),
        .S(S),
        .Q(Q),
        .R(R),
        .F(F)
    );

    // 2ns clock
    always #1 clk = ~clk;

    initial begin
        clk = 0;
        A = 0;
        B = 0;
        S = 0;

        // Wait for clock to start
        @(posedge clk);
        A = 16;
        B = 16'b0000011001011100;
        S = 1;

        @(posedge clk);
        S = 0;

        // Wait for F = 1
        repeat (50) begin
            @(posedge clk);
            if (F) begin
                $display("Inputs: A = %0d, B = %0d", A, B);
                $display("Q (16-bit): %0d", Q);
                $display("R (16-bit): %0d", R);
                $display("F: %0b", F);
                $finish;
            end
        end

        $display("ERROR: Timeout waiting for F = 1");
        $finish;
    end

endmodule
