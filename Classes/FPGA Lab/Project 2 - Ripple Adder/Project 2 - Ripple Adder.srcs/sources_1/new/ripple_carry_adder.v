`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Author: Amna Malik, R11827604
// Date: 9/8/2026
//
// Project: Ripple Ladder
//
// This module creates a 4-bit ripple carry adder.
// It uses four 1-bit full adders connected together.
// Each carry-out becomes the carry-in for the next full adder.
//
//////////////////////////////////////////////////////////////////////////////////

module ripple_carry_adder(
    input [3:0] A,
    input [3:0] B,
    input Cin,
    output [3:0] Sum,
    output Cout
    );

    // Wires used to carry between each full adder
    wire carry1;
    wire carry2;
    wire carry3;


    // First full adder, handles bit 0
    // Uses the original carry-in
    full_adder FA0(
        .A(A[0]),
        .B(B[0]),
        .Cin(Cin),
        .Sum(Sum[0]),
        .Cout(carry1)
    );


    // Second full adder, handles bit 1
    // Uses the carry from the first full adder
    full_adder FA1(
        .A(A[1]),
        .B(B[1]),
        .Cin(carry1),
        .Sum(Sum[1]),
        .Cout(carry2)
    );


    // Third full adder, handles bit 2
    // Uses the carry from the second full adder
    full_adder FA2(
        .A(A[2]),
        .B(B[2]),
        .Cin(carry2),
        .Sum(Sum[2]),
        .Cout(carry3)
    );


    // Fourth full adder, handles bit 3
    // Uses the carry from the third full adder
    // Its carry-out becomes the final Cout
    full_adder FA3(
        .A(A[3]),
        .B(B[3]),
        .Cin(carry3),
        .Sum(Sum[3]),
        .Cout(Cout)
    );

endmodule