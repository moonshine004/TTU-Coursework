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

    // =========================================================
    // CAMERA CONTROL
    // =========================================================

    // PWDN is active high.
    // Keep camera enabled.
    assign cam_pwdn = 1'b0;


    // =========================================================
    // SIOD OPEN-DRAIN CONTROL
    // =========================================================

    reg siod_drive_low;

    // Pull SIOD low when required.
    // Otherwise release the line and let the pull-up drive it high.
    assign cam_siod =
        siod_drive_low ? 1'b0 : 1'bz;


    // =========================================================
    // SCCB TIMING
    //
    // System clock = 100 MHz
    //
    // Generate a slower state-machine tick.
    // 100 MHz / 250 = 400 kHz tick rate.
    //
    // Several ticks are used per SCCB bit, so SIOC itself
    // runs comfortably below 400 kHz.
    // =========================================================

    reg [7:0] phase_counter;
    reg       phase_tick;

    always @(posedge clk or posedge reset) begin

        if (reset) begin

            phase_counter <= 8'd0;
            phase_tick    <= 1'b0;

        end

        else begin

            if (phase_counter == 8'd249) begin

                phase_counter <= 8'd0;
                phase_tick    <= 1'b1;

            end

            else begin

                phase_counter <= phase_counter + 1'b1;
                phase_tick    <= 1'b0;

            end

        end

    end


    // =========================================================
    // OV7670 REGISTER TABLE
    //
    // [15:8] = register address
    // [7:0]  = value
    //
    // This configuration requests:
    //
    // 640 x 480 VGA
    // RGB565
    // no QVGA downsampling
    // =========================================================

    localparam integer CFG_COUNT = 52;

    function [15:0] config_word;

        input [5:0] index;

        begin

            case (index)

                // -------------------------------------------------
                // RESET
                // -------------------------------------------------

                 0: config_word = {8'h12, 8'h80};


                // -------------------------------------------------
                // CLOCK / FORMAT
                // -------------------------------------------------

                // Nominal VGA frame-rate clock configuration
                 1: config_word = {8'h11, 8'h01};

                // Byte ordering
                 2: config_word = {8'h3A, 8'h04};

                // VGA + RGB
                 3: config_word = {8'h12, 8'h04};


                // -------------------------------------------------
                // FULL 640 x 480 VGA WINDOW
                // -------------------------------------------------

                 4: config_word = {8'h17, 8'h13};
                 5: config_word = {8'h18, 8'h01};
                 6: config_word = {8'h32, 8'hB6};

                 7: config_word = {8'h19, 8'h02};
                 8: config_word = {8'h1A, 8'h7A};
                 9: config_word = {8'h03, 8'h0A};


                // -------------------------------------------------
                // DISABLE QVGA DOWNSAMPLING
                // -------------------------------------------------

                10: config_word = {8'h0C, 8'h00};
                11: config_word = {8'h3E, 8'h00};

                12: config_word = {8'h70, 8'h3A};
                13: config_word = {8'h71, 8'h35};

                14: config_word = {8'h72, 8'h11};
                15: config_word = {8'h73, 8'hF0};

                16: config_word = {8'hA2, 8'h02};

                // Keep PCLK running during blanking
                17: config_word = {8'h15, 8'h00};


                // -------------------------------------------------
                // RGB565
                // -------------------------------------------------

                // Disable RGB444
                18: config_word = {8'h8C, 8'h00};

                // RGB565 + full output range
                19: config_word = {8'h40, 8'hD0};

                // Rewrite clock divider after format setup
                20: config_word = {8'h11, 8'h01};


                // -------------------------------------------------
                // COLOR / AUTOMATIC CONTROL
                // -------------------------------------------------

                21: config_word = {8'h14, 8'h38};

                22: config_word = {8'h4F, 8'h80};
                23: config_word = {8'h50, 8'h80};
                24: config_word = {8'h51, 8'h00};
                25: config_word = {8'h52, 8'h22};
                26: config_word = {8'h53, 8'h5E};
                27: config_word = {8'h54, 8'h80};
                28: config_word = {8'h58, 8'h9E};

                29: config_word = {8'h3D, 8'hC0};

                // AGC / AWB / AEC
                30: config_word = {8'h13, 8'hE7};

                31: config_word = {8'h41, 8'h08};
                32: config_word = {8'h6F, 8'h9F};

                33: config_word = {8'hB0, 8'h84};

                // Brightness
                34: config_word = {8'h55, 8'h00};

                // Contrast
                35: config_word = {8'h56, 8'h40};


                // -------------------------------------------------
                // GAMMA CURVE
                // -------------------------------------------------

                36: config_word = {8'h7A, 8'h20};
                37: config_word = {8'h7B, 8'h10};
                38: config_word = {8'h7C, 8'h1E};
                39: config_word = {8'h7D, 8'h35};
                40: config_word = {8'h7E, 8'h5A};
                41: config_word = {8'h7F, 8'h69};
                42: config_word = {8'h80, 8'h76};
                43: config_word = {8'h81, 8'h80};
                44: config_word = {8'h82, 8'h88};
                45: config_word = {8'h83, 8'h8F};
                46: config_word = {8'h84, 8'h96};
                47: config_word = {8'h85, 8'hA3};
                48: config_word = {8'h86, 8'hAF};
                49: config_word = {8'h87, 8'hC4};
                50: config_word = {8'h88, 8'hD7};
                51: config_word = {8'h89, 8'hE8};

                default:
                    config_word = 16'h0000;

            endcase

        end

    endfunction


    // =========================================================
    // SCCB STATE MACHINE
    // =========================================================

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


    // =========================================================
    // CONFIGURATION FSM
    // =========================================================

    always @(posedge clk or posedge reset) begin

        if (reset) begin

            state <= ST_HW_RESET;

            cam_rst_n <= 1'b0;

            cam_sioc <= 1'b1;
            siod_drive_low <= 1'b0;

            config_done <= 1'b0;

            delay_counter <= 21'd0;

            config_index <= 6'd0;
            current_config <= 16'd0;

            tx_byte <= 8'd0;

            byte_number <= 2'd0;
            bit_number <= 3'd7;

        end

        else begin

            case (state)

                // -------------------------------------------------
                // HARDWARE RESET
                // -------------------------------------------------

                ST_HW_RESET: begin

                    cam_rst_n <= 1'b0;

                    cam_sioc <= 1'b1;
                    siod_drive_low <= 1'b0;

                    config_done <= 1'b0;

                    // Hold reset low for about 1 ms
                    if (delay_counter == 21'd99_999) begin

                        delay_counter <= 21'd0;

                        cam_rst_n <= 1'b1;

                        state <= ST_RESET_WAIT;

                    end

                    else begin

                        delay_counter <= delay_counter + 1'b1;

                    end

                end


                // -------------------------------------------------
                // WAIT AFTER HARDWARE RESET
                // -------------------------------------------------

                ST_RESET_WAIT: begin

                    // Wait about 10 ms
                    if (delay_counter == 21'd999_999) begin

                        delay_counter <= 21'd0;
                        config_index <= 6'd0;

                        state <= ST_LOAD;

                    end

                    else begin

                        delay_counter <= delay_counter + 1'b1;

                    end

                end


                // -------------------------------------------------
                // LOAD NEXT REGISTER WRITE
                //
                // SCCB write sequence:
                //
                // 0x42
                // register address
                // register value
                // -------------------------------------------------

                ST_LOAD: begin

                    current_config <=
                        config_word(config_index);

                    // OV7670 SCCB write address
                    tx_byte <= 8'h42;

                    byte_number <= 2'd0;
                    bit_number <= 3'd7;

                    cam_sioc <= 1'b1;
                    siod_drive_low <= 1'b0;

                    state <= ST_START_A;

                end


                // -------------------------------------------------
                // START CONDITION
                // -------------------------------------------------

                ST_START_A: begin

                    if (phase_tick) begin

                        cam_sioc <= 1'b1;
                        siod_drive_low <= 1'b0;

                        state <= ST_START_B;

                    end

                end


                ST_START_B: begin

                    if (phase_tick) begin

                        // SIOD falls while SIOC is high
                        siod_drive_low <= 1'b1;

                        state <= ST_BIT_LOW;

                    end

                end


                // -------------------------------------------------
                // SEND ONE DATA BIT
                // -------------------------------------------------

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

                            bit_number <= bit_number - 1'b1;

                            state <= ST_BIT_LOW;

                        end

                    end

                end


                // -------------------------------------------------
                // SCCB NINTH CLOCK / ACK PHASE
                // -------------------------------------------------

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

                        // First transmitted byte was device address
                        if (byte_number == 0) begin

                            byte_number <= 2'd1;

                            tx_byte <= current_config[15:8];

                            bit_number <= 3'd7;

                            state <= ST_BIT_LOW;

                        end

                        // Second transmitted byte was register address
                        else if (byte_number == 1) begin

                            byte_number <= 2'd2;

                            tx_byte <= current_config[7:0];

                            bit_number <= 3'd7;

                            state <= ST_BIT_LOW;

                        end

                        // Third transmitted byte was register data
                        else begin

                            state <= ST_STOP_LOW;

                        end

                    end

                end


                // -------------------------------------------------
                // STOP CONDITION
                // -------------------------------------------------

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

                        // SIOD rises while SIOC is high
                        siod_drive_low <= 1'b0;

                        delay_counter <= 21'd0;

                        state <= ST_WRITE_DELAY;

                    end

                end


                // -------------------------------------------------
                // DELAY BETWEEN REGISTER WRITES
                // -------------------------------------------------

                ST_WRITE_DELAY: begin

                    // Longer delay after software reset
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

                        delay_counter <= 21'd0;

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

                        delay_counter <= delay_counter + 1'b1;

                    end

                end


                // -------------------------------------------------
                // CONFIGURATION COMPLETE
                // -------------------------------------------------

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