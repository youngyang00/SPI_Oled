`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/12/01 17:59:16
// Design Name: 
// Module Name: top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top(
input  clock, //100MHz onboard clock
input  reset,
//oled interface
output oled_spi_clk,
output oled_spi_data,
output oled_vdd,
output oled_vbat,
output oled_reset_n,
output oled_dc_n,
output oled_cs,

input [7:0] sendData,
input sendDataValid,
output sendDone
    );

/*localparam myString = "Hello world";
localparam StringLen = 11;
integer byteCounter;

reg [1:0]   state;
reg [7:0]   sendData;
reg         sendDataValid;
wire        sendDone;

localparam  IDLE = 'd0,
            SEND = 'd1,
            DONE = 'd2;

always @(posedge clock) begin
    if (reset) begin
        state <= IDLE;
        byteCounter <= StringLen;
        sendDataValid <= 1'b0;
    end
    else begin
        case (state)
            IDLE:begin
                if (!sendDone) begin
                    sendData <= myString[(byteCounter*8-1)-:8];
                    sendDataValid <= 1'b1;
                    state <= SEND;
                end
            end 
            SEND:begin
                if (sendDone) begin
                    sendDataValid <= 1'b0;
                    byteCounter <= byteCounter-1;
                    if (byteCounter != 1) begin
                        state <= IDLE;
                    end
                    else begin
                        state <= DONE;
                    end
                end
            end
            DONE:begin
                state <= DONE;
            end
        endcase
    end
end*/

oledControl OC(
    .clock(clock), //100MHz onboard clock
    .reset(reset),
//oled interface
    .oled_spi_clk(oled_spi_clk),
    .oled_spi_data(oled_spi_data),
    .oled_vdd(oled_vdd),
    .oled_vbat(oled_vbat),
    .oled_reset_n(oled_reset_n),
    .oled_dc_n(oled_dc_n),
    .oled_cs(oled_cs),
//
    .sendData(sendData),
    .sendDataValid(sendDataValid),
    .sendDone(sendDone)
    );
endmodule
