`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Author: Amna Malik, R11827604
// Date: 9/8/2026
//
// Project: Ripple Ladder
//
// This module creates a 1-bit full adder.
// It adds two input bits along with a carry-in.
// The outputs are the sum and carry-out.
//
//////////////////////////////////////////////////////////////////////////////////

module full_adder(
    input A,
    input B,
    input Cin,
    output Sum,
    output Cout
    );

    // XOR the three inputs together to find the sum
    assign Sum = A ^ B ^ Cin;

    // Carry out happens when at least two inputs are 1
    assign Cout = (A & B) | (A & Cin) | (B & Cin);

endmodule