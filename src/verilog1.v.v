`timescale 1ns / 1ps
module verilog1.v(c, a, b, x, z);
	input [7:0] a, b, c;
	output [7:0] z;
	output [15:0] x;
	wire [7:0] d, e;
	wire [15:0] f, g;
	wire [15:0] xwire;
	SADD #(.WIDTH(8))line1(a,b,d);
	SADD #(.WIDTH(8))line2(a,c,e);
	SCOMP #(.WIDTH(8))line3(d,e,g,1'b0,1'b0);
	SMUX2x1 #(.WIDTH(8))line4(d,e,g,z);
	SMUL #(.WIDTH(16))line5({{8{a[7]}},a[7:0]},{{8{c[7]}},c[7:0]},f);
	SSUB #(.WIDTH(16))line6(f,{{8{d[7]}},d[7:0]},xwire);
	REG #(.WIDTH(16))line7(CLK,RST,xwire,x);
endmodule