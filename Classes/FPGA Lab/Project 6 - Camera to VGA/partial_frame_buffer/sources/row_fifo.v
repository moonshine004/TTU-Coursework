`timescale 1ns / 1ps

module row_fifo (
    input  wire        reset,

    // Camera write side
    input  wire        write_clk,
    input  wire        write_enable,
    input  wire [15:0] write_data,

    output wire        full,
    output wire        overflow,
    output wire        write_reset_busy,
    output wire [16:0] write_count,

    // VGA read side
    input  wire        read_clk,
    input  wire        read_enable,

    output wire [15:0] read_data,

    output wire        empty,
    output wire        underflow,
    output wire        read_reset_busy,
    output wire [16:0] read_count
);

    wire almost_empty;
    wire almost_full;

    wire prog_empty;
    wire prog_full;

    wire data_valid;
    wire wr_ack;

    wire sbiterr;
    wire dbiterr;


    // 65536 pixels x 16 bits
    // About 102 rows of 640 pixels.

    xpm_fifo_async #(
        .CDC_SYNC_STAGES(2),
        .DOUT_RESET_VALUE("0"),
        .ECC_MODE("no_ecc"),
        .FIFO_MEMORY_TYPE("block"),
        .FIFO_READ_LATENCY(0),
        .FIFO_WRITE_DEPTH(65536),
        .FULL_RESET_VALUE(0),

        .PROG_EMPTY_THRESH(1024),
        .PROG_FULL_THRESH(64512),

        .RD_DATA_COUNT_WIDTH(17),
        .READ_DATA_WIDTH(16),
        .READ_MODE("fwft"),

        .RELATED_CLOCKS(0),
        .SIM_ASSERT_CHK(0),

        .USE_ADV_FEATURES("0707"),

        .WAKEUP_TIME(0),

        .WRITE_DATA_WIDTH(16),
        .WR_DATA_COUNT_WIDTH(17)
    )
    camera_row_fifo (
        .sleep(1'b0),
        .rst(reset),

        // Write side
        .wr_clk(write_clk),
        .wr_en(write_enable),
        .din(write_data),

        .full(full),
        .almost_full(almost_full),
        .prog_full(prog_full),

        .wr_data_count(write_count),

        .overflow(overflow),
        .wr_ack(wr_ack),

        .wr_rst_busy(write_reset_busy),

        // Read side
        .rd_clk(read_clk),
        .rd_en(read_enable),

        .dout(read_data),

        .empty(empty),
        .almost_empty(almost_empty),
        .prog_empty(prog_empty),

        .rd_data_count(read_count),

        .underflow(underflow),
        .data_valid(data_valid),

        .rd_rst_busy(read_reset_busy),

        // ECC unused
        .injectsbiterr(1'b0),
        .injectdbiterr(1'b0),

        .sbiterr(sbiterr),
        .dbiterr(dbiterr)
    );

endmodule
