`timescale 1ns / 1ps
module verilogFile(c, a, b, x, z);
	input [63:0] a, b, c;
	output [31:0] z, x;
	wire [63:0] d, e, f, g, h;
	wire dLTe, dEQe;
	wire [63:0] xrin, zrin;
	reg [63:0] greg, hreg;
	SADD #(.WIDTH(64))line1(a,b,d);
	SADD #(.WIDTH(64))line2(a,c,e);
	SSUB #(.WIDTH(64))line3(a,b,f);
	SCOMP #(.WIDTH(64))line4(d,e,1'b0,1'b0,dEQe);
	SCOMP #(.WIDTH(64))line5(d,e,1'b0,dLTe,1'b0);
	SMUX2x1 #(.WIDTH(64))line6(d,e,dLTe,g);
	SMUX2x1 #(.WIDTH(64))line7(g,f,dEQe,h);
	REG #(.WIDTH(64))line8(CLK,RST,g,greg);
	REG #(.WIDTH(64))line9(CLK,RST,h,hreg);
	SSHL #(.WIDTH(64))line10(hreg,{63'b0,dLTe},xrin);
	SSHR #(.WIDTH(64))line11(greg,{63'b0,dEQe},zrin);
	REG #(.WIDTH(32))line12(CLK,RST,xrin[31:0],x);
	REG #(.WIDTH(32))line13(CLK,RST,zrin[31:0],z);
endmodule