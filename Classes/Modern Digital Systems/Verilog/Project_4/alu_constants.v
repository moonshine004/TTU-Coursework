// alu_constants.v - Common constants for the ALU project
module alu_constants;
    // Operation codes
    parameter ADD_OP      = 3'b000;
    parameter SUBTRACT_OP = 3'b001;
    parameter AND_OP      = 3'b010;
    parameter OR_OP       = 3'b011;
    parameter XOR_OP      = 3'b100;
    parameter INVERT_OP   = 3'b101;
    parameter SHIFT_L_OP  = 3'b110;
    parameter SHIFT_R_OP  = 3'b111;
    
    // You can add other common constants here if needed
endmodule
