`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Author: Amna Malik, R11827604
// Date: 9/8/2026
//
// Project: Ripple Ladder
//
// This testbench checks the 1-bit full adder.
// It tests every possible combination of A, B, and Cin.
// The Sum and Cout outputs are checked in simulation.
//
//////////////////////////////////////////////////////////////////////////////////

module full_adder_tb();

    // Inputs that we will change during the simulation
    reg A;
    reg B;
    reg Cin;

    // Outputs coming from the full adder
    wire Sum;
    wire Cout;

    // This is the full adder we are testing
    full_adder DUT(
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );

    // Run through every possible input combination
    initial begin

        // Test 000
        A = 0;
        B = 0;
        Cin = 0;
        #10;

        // Test 001
        A = 0;
        B = 0;
        Cin = 1;
        #10;

        // Test 010
        A = 0;
        B = 1;
        Cin = 0;
        #10;

        // Test 011
        A = 0;
        B = 1;
        Cin = 1;
        #10;

        // Test 100
        A = 1;
        B = 0;
        Cin = 0;
        #10;

        // Test 101
        A = 1;
        B = 0;
        Cin = 1;
        #10;

        // Test 110
        A = 1;
        B = 1;
        Cin = 0;
        #10;

        // Test 111
        A = 1;
        B = 1;
        Cin = 1;
        #10;

        // End the simulation
        $finish;

    end

endmodule