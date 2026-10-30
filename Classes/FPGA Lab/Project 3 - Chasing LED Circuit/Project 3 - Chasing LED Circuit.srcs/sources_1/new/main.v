`timescale 1ns / 1ps

module main(
    input wire CLK100MHZ,
    input wire rst,
    input wire init_btn,
    output wire [15:0] LED,
    output reg [6:0] seg,
    output wire [3:0] an,
    output wire dp
);

    ////////////////////////////////
    // Creating a 5 Hz Movement Tick
    ////////////////////////////////
    reg [24:0] counter = 0;
    reg move_tick = 0;
    
    always @(posedge CLK100MHZ) begin
        if (rst) begin
            counter <= 0;
            move_tick <= 0;
        end
        
        else if (counter == 9999999) begin
            counter <= 0;
            move_tick <= 1;
        end
        
        else begin
            counter <= counter + 1;
            move_tick <= 0;
        end
    end
    
    
    ////////////////////////////////
    // Internal Signals
    ////////////////////////////////
    wire [15:0] d;
    wire [15:0] q;
    
    reg dir;
    wire move_dir;
    
    reg init_prev;
    wire init_pulse;
    
    wire dff_enable;
    
    reg [3:0] laps = 4'b0000;
    
    ////////////////////////////////
    // Initialize Button
    ////////////////////////////////
    assign init_pulse = init_btn & ~init_prev & ~rst;
    
    always @(posedge CLK100MHZ) begin
        if (rst)
            init_prev <= 1'b0;
            
        else
            init_prev <= init_btn;
    end
    
    
    ////////////////////////////////
    // DFF Enable
    ////////////////////////////////
    assign dff_enable = move_tick | init_pulse;
    
    
    ////////////////////////////////
    // Direction Logic
    ////////////////////////////////
    assign move_dir = (q == 16'b1000000000000000) ? 1'b0 :
                      (q == 16'b0000000000000001) ? 1'b1 :
                      dir;
                      
    always @(posedge CLK100MHZ) begin
        if (rst)
            dir <= 1'b1;
            
        else
            dir <= move_dir;
    end
    
    
    ////////////////////////////////
    // Assigning Directional Pathways
    ////////////////////////////////
    assign d = init_pulse ? 16'b0000000000000001 :
           move_dir   ? (q << 1) :
                        (q >> 1);
    
    
    ////////////////////////////////
    // Creating 16 D Flip Flops
    ////////////////////////////////
    genvar i;
    
    generate 
        for (i = 0; i < 16; i = i + 1) begin : dff_loop
            dff dff_inst(
                .d(d[i]),
                .rst(rst),
                .enable(dff_enable),
                .clk(CLK100MHZ),
                .q(q[i])
                );
        end
    endgenerate
    
    ////////////////////////////////
    // LED Outputs
    ////////////////////////////////
    assign LED = q;
    
    ////////////////////////////////
// Lap Counter
////////////////////////////////
    always @(posedge CLK100MHZ) begin
        if (rst)
            laps <= 4'd0;
    
        // if move_tick is true AND q is ...0010 AND move_dir is 0
        else if (move_tick &&
                 q == 16'b0000000000000010 &&
                 move_dir == 1'b0) begin
    
            if (laps == 4'd9)
                laps <= 4'd0;
    
            else
                laps <= laps + 1'b1;
        end
    end
    
        ////////////////////////////////
    // Seven Segment Display
    ////////////////////////////////

    // Turn on only the rightmost digit
    assign an = 4'b1110;

    // Turn decimal point off
    assign dp = 1'b1;

    always @(*) begin
        case (laps)
            4'd0: seg = 7'b1000000;
            4'd1: seg = 7'b1111001;
            4'd2: seg = 7'b0100100;
            4'd3: seg = 7'b0110000;
            4'd4: seg = 7'b0011001;
            4'd5: seg = 7'b0010010;
            4'd6: seg = 7'b0000010;
            4'd7: seg = 7'b1111000;
            4'd8: seg = 7'b0000000;
            4'd9: seg = 7'b0010000;
            default: seg = 7'b1111111;
        endcase
    end
    
    
endmodule