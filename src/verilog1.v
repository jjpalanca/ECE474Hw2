`timescale 1ns / 1ps
module verilog1(sa, g, h, b, d, f, a, c, e, avg);
	input [15:0] a, b, c, d, e, f, g, h;
	input [7:0] sa;
	output [15:0] avg;
	wire [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;
	SADD #(.WIDTH(32))line1({{16{a[15]}},a[15:0]},{{16{b[15]}},b[15:0]},l00);
	SADD #(.WIDTH(32))line2({{16{c[15]}},c[15:0]},{{16{d[15]}},d[15:0]},l01);
	SADD #(.WIDTH(32))line3({{16{e[15]}},e[15:0]},{{16{f[15]}},f[15:0]},l02);
	SADD #(.WIDTH(32))line4({{16{g[15]}},g[15:0]},{{16{h[15]}},h[15:0]},l03);
	SADD #(.WIDTH(32))line5(l00,l01,l10);
	SADD #(.WIDTH(32))line6(l02,l03,l11);
	SADD #(.WIDTH(32))line7(l10,l11,l2);
	SSHR #(.WIDTH(32))line8(l2,{{24{sa[7]}},sa[7:0]},l2div2);
	SSHR #(.WIDTH(32))line9(l2div2,{{24{sa[7]}},sa[7:0]},l2div4);
	SSHR #(.WIDTH(32))line10(l2div4,{{24{sa[7]}},sa[7:0]},l2div8);
	REG #(.WIDTH(16))line11(CLK,RST,l2div8[15:0],avg);
endmodule