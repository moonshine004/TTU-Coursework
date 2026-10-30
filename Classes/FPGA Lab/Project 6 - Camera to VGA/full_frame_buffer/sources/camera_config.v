`timescale 1ns / 1ps

module camera_config (
    input  wire clk,
    input  wire reset,

    output reg  cam_sioc,
    inout  wire cam_siod,

    output reg  cam_rst_n,
    output wire cam_pwdn,

    output reg  config_done
);

    // ---------------------------------------------------------
    // CAMERA POWER CONTROL
    // ---------------------------------------------------------

    // PWDN is active high.
    // Keep camera enabled.
    assign cam_pwdn = 1'b0;


    // ---------------------------------------------------------
    // SIOD OPEN-DRAIN CONTROL
    // ---------------------------------------------------------

    reg siod_drive_low;

    assign cam_siod =
        siod_drive_low ? 1'b0 : 1'bz;


    // ---------------------------------------------------------
    // SCCB STATE TIMING
    // ---------------------------------------------------------

    reg [7:0] phase_counter;
    reg       phase_tick;

    always @(posedge clk or posedge reset) begin

        if (reset) begin
            phase_counter <= 0;
            phase_tick    <= 0;
        end

        else begin

            if (phase_counter == 8'd249) begin

                phase_counter <= 0;
                phase_tick    <= 1'b1;

            end

            else begin

                phase_counter <= phase_counter + 1'b1;
                phase_tick    <= 1'b0;

            end

        end

    end


    // ---------------------------------------------------------
    // OV7670 CONFIGURATION TABLE
    //
    // [15:8] = register
    // [7:0]  = value
    // ---------------------------------------------------------

    localparam integer CFG_COUNT = 51;

    function [15:0] config_word;

        input [5:0] index;

        begin

            case (index)

                // Software reset
                 0: config_word = {8'h12, 8'h80};

                // Clock divider
                 1: config_word = {8'h11, 8'h01};

                // Output byte ordering
                 2: config_word = {8'h3A, 8'h04};

                // RGB mode
                 3: config_word = {8'h12, 8'h04};


                // ---------------------------------------------
                // CAMERA WINDOW
                // ---------------------------------------------

                 4: config_word = {8'h17, 8'h16};
                 5: config_word = {8'h18, 8'h04};
                 6: config_word = {8'h32, 8'h24};

                 7: config_word = {8'h19, 8'h02};
                 8: config_word = {8'h1A, 8'h7A};
                 9: config_word = {8'h03, 8'h0A};


                // ---------------------------------------------
                // QVGA 320 x 240
                // ---------------------------------------------

                10: config_word = {8'h0C, 8'h04};
                11: config_word = {8'h3E, 8'h19};

                12: config_word = {8'h70, 8'h3A};
                13: config_word = {8'h71, 8'h35};

                14: config_word = {8'h72, 8'h11};
                15: config_word = {8'h73, 8'hF1};

                16: config_word = {8'hA2, 8'h02};

                // Keep PCLK active during blanking
                17: config_word = {8'h15, 8'h00};


                // ---------------------------------------------
                // RGB565
                // ---------------------------------------------

                // Disable RGB444
                18: config_word = {8'h8C, 8'h00};

                // RGB565 + full output range
                19: config_word = {8'h40, 8'hD0};


                // ---------------------------------------------
                // COLOR / AUTOMATIC CONTROL
                // ---------------------------------------------

                20: config_word = {8'h14, 8'h38};

                21: config_word = {8'h4F, 8'h80};
                22: config_word = {8'h50, 8'h80};
                23: config_word = {8'h51, 8'h00};
                24: config_word = {8'h52, 8'h22};
                25: config_word = {8'h53, 8'h5E};
                26: config_word = {8'h54, 8'h80};
                27: config_word = {8'h58, 8'h9E};

                28: config_word = {8'h3D, 8'hC0};

                // AGC / AWB / AEC
                29: config_word = {8'h13, 8'hE7};

                30: config_word = {8'h41, 8'h08};
                31: config_word = {8'h6F, 8'h9F};

                32: config_word = {8'hB0, 8'h84};

                // Brightness
                33: config_word = {8'h55, 8'h00};

                // Contrast
                34: config_word = {8'h56, 8'h40};


                // ---------------------------------------------
                // GAMMA CURVE
                // ---------------------------------------------

                35: config_word = {8'h7A, 8'h20};
                36: config_word = {8'h7B, 8'h10};
                37: config_word = {8'h7C, 8'h1E};
                38: config_word = {8'h7D, 8'h35};
                39: config_word = {8'h7E, 8'h5A};
                40: config_word = {8'h7F, 8'h69};
                41: config_word = {8'h80, 8'h76};
                42: config_word = {8'h81, 8'h80};
                43: config_word = {8'h82, 8'h88};
                44: config_word = {8'h83, 8'h8F};
                45: config_word = {8'h84, 8'h96};
                46: config_word = {8'h85, 8'hA3};
                47: config_word = {8'h86, 8'hAF};
                48: config_word = {8'h87, 8'hC4};
                49: config_word = {8'h88, 8'hD7};
                50: config_word = {8'h89, 8'hE8};

                default:
                    config_word = 16'h0000;

            endcase

        end

    endfunction


    // ---------------------------------------------------------
    // SCCB FSM STATES
    // ---------------------------------------------------------

    localparam ST_HW_RESET       = 5'd0;
    localparam ST_RESET_WAIT     = 5'd1;

    localparam ST_LOAD           = 5'd2;

    localparam ST_START_A        = 5'd3;
    localparam ST_START_B        = 5'd4;

    localparam ST_BIT_LOW        = 5'd5;
    localparam ST_BIT_HIGH       = 5'd6;
    localparam ST_BIT_FALL       = 5'd7;

    localparam ST_ACK_LOW        = 5'd8;
    localparam ST_ACK_HIGH       = 5'd9;
    localparam ST_ACK_FALL       = 5'd10;

    localparam ST_STOP_LOW       = 5'd11;
    localparam ST_STOP_HIGH      = 5'd12;
    localparam ST_STOP_RELEASE   = 5'd13;

    localparam ST_WRITE_DELAY    = 5'd14;

    localparam ST_DONE           = 5'd15;


    reg [4:0] state;

    reg [20:0] delay_counter;

    reg [5:0] config_index;

    reg [15:0] current_config;

    reg [7:0] tx_byte;

    reg [1:0] byte_number;
    reg [2:0] bit_number;


    always @(posedge clk or posedge reset) begin

        if (reset) begin

            state <= ST_HW_RESET;

            cam_rst_n <= 1'b0;

            cam_sioc <= 1'b1;
            siod_drive_low <= 1'b0;

            config_done <= 1'b0;

            delay_counter <= 0;

            config_index <= 0;
            current_config <= 0;

            tx_byte <= 0;

            byte_number <= 0;
            bit_number <= 7;

        end

        else begin

            case (state)

                // ---------------------------------------------
                // HARDWARE RESET
                // ---------------------------------------------

                ST_HW_RESET: begin

                    cam_rst_n <= 1'b0;

                    cam_sioc <= 1'b1;
                    siod_drive_low <= 1'b0;

                    config_done <= 1'b0;

                    if (delay_counter == 21'd99_999) begin

                        delay_counter <= 0;

                        cam_rst_n <= 1'b1;

                        state <= ST_RESET_WAIT;

                    end

                    else begin

                        delay_counter <=
                            delay_counter + 1'b1;

                    end

                end


                // ---------------------------------------------
                // WAIT AFTER RESET
                // ---------------------------------------------

                ST_RESET_WAIT: begin

                    if (delay_counter == 21'd999_999) begin

                        delay_counter <= 0;
                        config_index <= 0;

                        state <= ST_LOAD;

                    end

                    else begin

                        delay_counter <=
                            delay_counter + 1'b1;

                    end

                end


                // ---------------------------------------------
                // LOAD REGISTER WRITE
                // ---------------------------------------------

                ST_LOAD: begin

                    current_config <=
                        config_word(config_index);

                    // OV7670 SCCB write address
                    tx_byte <= 8'h42;

                    byte_number <= 0;
                    bit_number <= 7;

                    cam_sioc <= 1'b1;
                    siod_drive_low <= 1'b0;

                    state <= ST_START_A;

                end


                // ---------------------------------------------
                // START CONDITION
                // ---------------------------------------------

                ST_START_A: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b1;
                        siod_drive_low <= 1'b0;

                        state <= ST_START_B;

                    end

                end


                ST_START_B: begin

                    if (phase_tick) begin

                        siod_drive_low <= 1'b1;

                        state <= ST_BIT_LOW;

                    end

                end


                // ---------------------------------------------
                // DATA BIT
                // ---------------------------------------------

                ST_BIT_LOW: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b0;

                        if (tx_byte[bit_number] == 1'b0)
                            siod_drive_low <= 1'b1;
                        else
                            siod_drive_low <= 1'b0;

                        state <= ST_BIT_HIGH;

                    end

                end


                ST_BIT_HIGH: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b1;

                        state <= ST_BIT_FALL;

                    end

                end


                ST_BIT_FALL: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b0;

                        if (bit_number == 0) begin

                            state <= ST_ACK_LOW;

                        end

                        else begin

                            bit_number <=
                                bit_number - 1'b1;

                            state <= ST_BIT_LOW;

                        end

                    end

                end


                // ---------------------------------------------
                // ACK / NINTH CLOCK
                // ---------------------------------------------

                ST_ACK_LOW: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b0;

                        // Release SIOD
                        siod_drive_low <= 1'b0;

                        state <= ST_ACK_HIGH;

                    end

                end


                ST_ACK_HIGH: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b1;

                        state <= ST_ACK_FALL;

                    end

                end


                ST_ACK_FALL: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b0;

                        if (byte_number == 0) begin

                            byte_number <= 1;

                            tx_byte <=
                                current_config[15:8];

                            bit_number <= 7;

                            state <= ST_BIT_LOW;

                        end

                        else if (byte_number == 1) begin

                            byte_number <= 2;

                            tx_byte <=
                                current_config[7:0];

                            bit_number <= 7;

                            state <= ST_BIT_LOW;

                        end

                        else begin

                            state <= ST_STOP_LOW;

                        end

                    end

                end


                // ---------------------------------------------
                // STOP CONDITION
                // ---------------------------------------------

                ST_STOP_LOW: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b0;
                        siod_drive_low <= 1'b1;

                        state <= ST_STOP_HIGH;

                    end

                end


                ST_STOP_HIGH: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b1;
                        siod_drive_low <= 1'b1;

                        state <= ST_STOP_RELEASE;

                    end

                end


                ST_STOP_RELEASE: begin

                    if (phase_tick) begin

                        siod_drive_low <= 1'b0;

                        delay_counter <= 0;

                        state <= ST_WRITE_DELAY;

                    end

                end


                // ---------------------------------------------
                // REGISTER SETTLING TIME
                // ---------------------------------------------

                ST_WRITE_DELAY: begin

                    if (
                        (
                            config_index == 0 &&
                            delay_counter >= 21'd499_999
                        )
                        ||
                        (
                            config_index != 0 &&
                            delay_counter >= 21'd99_999
                        )
                    ) begin

                        delay_counter <= 0;

                        if (
                            config_index ==
                            CFG_COUNT - 1
                        ) begin

                            state <= ST_DONE;

                        end

                        else begin

                            config_index <=
                                config_index + 1'b1;

                            state <= ST_LOAD;

                        end

                    end

                    else begin

                        delay_counter <=
                            delay_counter + 1'b1;

                    end

                end


                // ---------------------------------------------
                // FINISHED
                // ---------------------------------------------

                ST_DONE: begin

                    cam_sioc <= 1'b1;
                    siod_drive_low <= 1'b0;

                    config_done <= 1'b1;

                end


                default: begin

                    state <= ST_HW_RESET;

                end

            endcase

        end

    end

endmodule