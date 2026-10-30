`timescale 1ns / 1ps

module dff(
    input wire d,
    input wire rst,
    input wire clk,
    input wire enable,
    output reg q
    );
    
    parameter INIT = 1'b0;
    
    always @(posedge clk) begin
        if (rst == 1)
            q <= INIT;
            
        else if (enable == 1)
            q <= d;
            
        end        
        
        
endmodule
