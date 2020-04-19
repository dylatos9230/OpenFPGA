`timescale 1ns / 1ps

module top(
  a,
  b,
  clk,
  c,
  d);

input wire clk;

input wire a;
input wire b;
output wire c;
output reg d;

assign c = a & b;

always @(posedge clk) begin
  d <= c;
end

endmodule
