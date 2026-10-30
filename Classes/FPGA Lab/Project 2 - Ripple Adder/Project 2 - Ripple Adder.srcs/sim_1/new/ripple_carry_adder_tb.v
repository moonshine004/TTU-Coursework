`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Author: Amna Malik, R11827604
// Date: 9/8/2026
//
// Project: Ripple Ladder
//
// This testbench checks the 4-bit ripple carry adder.
// It tests different values for A, B, and Cin.
// The Sum and Cout outputs are checked in simulation.
//
//////////////////////////////////////////////////////////////////////////////////

module ripple_carry_adder_tb();

    // 4-bit inputs that we will change during the simulation
    reg [3:0] A;
    reg [3:0] B;

    // Carry-in input
    reg Cin;

    // Outputs coming from the ripple carry adder
    wire [3:0] Sum;
    wire Cout;


    // This is the ripple carry adder we are testing
    ripple_carry_adder DUT(
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );


    // Test different addition combinations
    initial begin

        // Test 1: 0 + 0 = 0
        A = 4'b0000;
        B = 4'b0000;
        Cin = 0;
        #10;


        // Test 2: 5 + 3 = 8
        A = 4'b0101;
        B = 4'b0011;
        Cin = 0;
        #10;


        // Test 3: 7 + 1 = 8
        // Checks the carry moving through multiple full adders
        A = 4'b0111;
        B = 4'b0001;
        Cin = 0;
        #10;


        // Test 4: 9 + 6 = 15
        A = 4'b1001;
        B = 4'b0110;
        Cin = 0;
        #10;


        // Test 5: 15 + 1 = 16
        // Sum should be 0000 and Cout should be 1
        A = 4'b1111;
        B = 4'b0001;
        Cin = 0;
        #10;


        // Test 6: 5 + 3 + Cin = 9
        // Checks that the carry-in works
        A = 4'b0101;
        B = 4'b0011;
        Cin = 1;
        #10;


        // Test 7: 15 + 15 = 30
        // Sum should be 1110 and Cout should be 1
        A = 4'b1111;
        B = 4'b1111;
        Cin = 0;
        #10;


        // Test 8: 15 + 15 + Cin = 31
        // Sum should be 1111 and Cout should be 1
        A = 4'b1111;
        B = 4'b1111;
        Cin = 1;
        #10;


        // End the simulation
        $finish;

    end

endmodule