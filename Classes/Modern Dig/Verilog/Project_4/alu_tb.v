`include "alu_top.v"
// alu_tb.v - Main project wrapper for the ALU
module alu_tb;
    reg [2:0] opA;
    reg [2:0] opB;
    reg [2:0] opS; 
    wire [7:0] Result;
integer i=0;
    // Instantiate the ALU
    alu_top main_alu (
        .opA(opA),
        .opB(opB),
        .opS(opS),
        .Result(Result)
    );

    initial begin
        opA = 8'd1;
        opB = 8'd5;
        opS = 3'b000;  // Addition
    for (i = 0; i < 8; i = i + 1) begin
            opS = i;
            #10; // Wait for the output to stabilize
            $display("OpS: %b, Result: %b", opS, Result);  
    end
    end

    // This could include more logic for interfacing with other components
    // or for setting up a test environment
endmodule

/**
1: Fill in modules
2: Create testbenches for all
3: Make adder (Look back at notes)
4: Make sure you're passing the right amount of bits to stuff
5: Make sure stuff runs
**/
