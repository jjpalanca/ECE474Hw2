`timescale 1ns / 1ps
module circuit3(b, a, c);
	input [31:0] a;
	input [15:0] b;
	output [7:0] c;
	wire [7:0] cwire;
	SADD #(.WIDTH(8))line1(a[7:0],b[7:0],cwire);
	REG #(.WIDTH(8))line2(CLK,RST,cwire,c);
endmodule