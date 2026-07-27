`include "alu_top.v"
// alu_top_tb.v - Testbench for the complete ALU
module alu_top_tb;
    // Inputs
    reg [7:0] opA;
    reg [7:0] opB;
    reg [2:0] opS;
    
    // Outputs
    wire [7:0] Result;
    
    // Instantiate the Unit Under Test (UUT)
    alu_top uut (
        .opA(opA),
        .opB(opB),
        .opS(opS),
        .Result(Result)
    );
    
    initial begin
        // Initialize inputs
        opA = 0;
        opB = 0;
        opS = 0;
        
        // Wait for global reset
        #100;
        
        // Test Addition: 25 + 40 = 65
        opA = 8'd25;
        opB = 8'd40;
        opS = 3'b000;  // Addition
        #10;f
        $display("Addition: %d + %d = %d", opA, opB, Result);
        
        // Test Subtraction: 50 - 30 = 20
        opA = 8'd50;
        opB = 8'd30;
        opS = 3'b001;  // Subtraction
        #10;
        $display("Subtraction: %d - %d = %d", opA, opB, Result);
        
        // Test AND: 0xF0 & 0x0F = 0x00
        opA = 8'hF0;
        opB = 8'h0F;
        opS = 3'b010;  // AND
        #10;
        $display("AND: 0x%h & 0x%h = 0x%h", opA, opB, Result);
        
        // Test OR: 0xF0 | 0x0F = 0xFF
        opA = 8'hF0;
        opB = 8'h0F;
        opS = 3'b011;  // OR
        #10;
        $display("OR: 0x%h | 0x%h = 0x%h", opA, opB, Result);
        
        // Test XOR: 0xFF ^ 0x0F = 0xF0
        opA = 8'hFF;
        opB = 8'h0F;
        opS = 3'b100;  // XOR
        #10;
        $display("XOR: 0x%h ^ 0x%h = 0x%h", opA, opB, Result);
        
        // Test INVERT: ~0xAA = 0x55
        opA = 8'hAA;
        opB = 8'h00;  // Not used
        opS = 3'b101;  // INVERT
        #10;
        $display("INVERT: ~0x%h = 0x%h", opA, Result);
        
        // Test Shift Left: 0x01 << 3 = 0x08
        opA = 8'h01;
        opB = 8'h03;  // Shift by 3
        opS = 3'b110;  // Shift Left
        #10;
        $display("Shift Left: 0x%h << %d = 0x%h", opA, opB[2:0], Result);
        
        // Test Shift Right: 0x80 >> 4 = 0x08
        opA = 8'h80;
        opB = 8'h04;  // Shift by 4
        opS = 3'b111;  // Shift Right
        #10;
        $display("Shift Right: 0x%h >> %d = 0x%h", opA, opB[2:0], Result);
        
        $finish;
    end
endmodule