`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//
// Author: Amna Malik, R11827604
// Date: 9/1/2026
//
// Project: FPGA Verification Demo
//
// This is a basic test project for the Basys 3.
// The top and bottom buttons start an LED wave in opposite directions.
// The center button resets everything.
//
//////////////////////////////////////////////////////////////////////////////////

module verification_demo(
    input  wire clk,          // 100 MHz Basys 3 clock
    input  wire btnU,         // Starts wave from LED 0 to LED 15
    input  wire btnD,         // Starts wave from LED 15 to LED 0
    input  wire btnC,         // Reset button
    output reg  [15:0] led    // 16 LEDs on the board
);


// The actual clock is WAY too fast for us to see the LEDs move
// so this counter slows everything down
reg [25:0] counter = 0;


// Keeps track of which LED we are currently on
// 4 bits gives us 0-15, which is exactly the 16 LEDs we have
reg [3:0] position = 0;


// Keeps track of if the wave is currently going
// 0 = not running
// 1 = running
reg running = 0;


// Keeps track of which way the wave is going
// 0 = LED 0 -> LED 15
// 1 = LED 15 -> LED 0
reg direction = 0;


// Main logic for the buttons and LED wave
// This runs on every rising edge of the 100 MHz clock
always @(posedge clk) begin


    // Center button is our reset
    // Clears everything and stops whatever wave is currently going
    if (btnC) begin
        counter   <= 0;
        position  <= 0;
        running   <= 0;
        direction <= 0;
    end


    // Top button starts at LED 0 and goes up to LED 15
    // !running makes sure we don't start another wave in the middle of one
    else if (!running && btnU) begin
        counter   <= 0;
        position  <= 0;
        running   <= 1;
        direction <= 0;
    end


    // Bottom button does the opposite
    // Starts at LED 15 and goes back down to LED 0
    else if (!running && btnD) begin
        counter   <= 0;
        position  <= 15;
        running   <= 1;
        direction <= 1;
    end


    // If we already started a wave, keep it going
    else if (running) begin


        // The Basys 3 clock is 100 MHz, or 100 million cycles per second
        // Waiting 10 million cycles gives us about 0.1 seconds per LED
        // Otherwise the whole wave would happen way too fast to actually see
        if (counter >= 9_999_999) begin

            // We hit our delay, so reset the counter for the next LED
            counter <= 0;


            // direction = 0 means we are going from LED 0 -> LED 15
            if (direction == 0) begin

                // LED 15 is the end, so once we get here the wave is done
                if (position == 15) begin
                    running <= 0;
                end

                // Haven't made it to LED 15 yet, move to the next LED
                else begin
                    position <= position + 1;
                end

            end


            // direction = 1 means we are going the other way
            // LED 15 -> LED 0
            else begin

                // LED 0 is the end going this direction
                if (position == 0) begin
                    running <= 0;
                end

                // Haven't made it to LED 0 yet, move back one LED
                else begin
                    position <= position - 1;
                end

            end

        end


        // Haven't waited long enough yet, so just keep counting
        else begin
            counter <= counter + 1;
        end

    end

end


// This decides which physical LED should actually be turned on
always @(*) begin

    if (running)

        // Start with LED 0 on:
        // 0000 0000 0000 0001
        //
        // Then shift that 1 over based on position
        // position = 0 -> 0000 0000 0000 0001
        // position = 1 -> 0000 0000 0000 0010
        // position = 2 -> 0000 0000 0000 0100
        //
        // So we only ever have one LED on at a time
        led = 16'b0000_0000_0000_0001 << position;

    else

        // No wave going = all LEDs off
        led = 16'b0000_0000_0000_0000;

end


endmodule