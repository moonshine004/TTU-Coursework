`include "add.v"
// add_tb.v - Testbench for Addition module
module add_tb;
    // Inputs
    reg [7:0] a;
    reg [7:0] b;
    
    // Outputs
    wire [7:0] result;
    
    // Instantiate the Unit Under Test (UUT)
    add uut (
        .a(a),
        .b(b),
        .result(result)
    );
    
    initial begin
        // Initialize inputs
        a = 0;
        b = 0;
        
        // Wait for global reset
        #100;
        
        // Test case 1: 5 + 10 = 15
        a = 8'd5;
        b = 8'd10;
        #10;
        if (result !== 8'd15) $display("Test case 1 failed: %d + %d = %d", a, b, result);
        else $display("Test case 1 passed");
        
        // Test case 2: 255 + 1 = 0 (overflow)
        a = 8'd255;
        b = 8'd1;
        #10;
        if (result !== 8'd0) $display("Test case 2 failed: %d + %d = %d", a, b, result);
        else $display("Test case 2 passed");
        
        // Test case 3: 128 + 128 = 0 (overflow)
        a = 8'd128;
        b = 8'd128;
        #10;
        if (result !== 8'd0) $display("Test case 3 failed: %d + %d = %d", a, b, result);
        else $display("Test case 3 passed");
        
        $finish;
    end
endmodule
