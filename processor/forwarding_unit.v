`timescale 1ns / 1ps

`include "defines.vh"

// parameterize register value size?
// this feels like something that should be defined at processor level ...
module forwarding_unit(
  id_ex_rs,
  id_ex_rt,

  ex_mem_rd0,
  mem_wb_rd0,
  ex_mem_reg_write0,
  mem_wb_reg_write0,

  ex_mem_rd1,
  mem_wb_rd1,
  ex_mem_reg_write1,
  mem_wb_reg_write1,

  forward_a0,
  forward_b0,

  forward_a1,
  forward_b1,
  );

  //parameter NUM_REGISTERS_LOG2 = $clog2(`NUM_REGISTERS);

  input wire [`NUM_REGISTERS_LOG2-1:0] id_ex_rs;
  input wire [`NUM_REGISTERS_LOG2-1:0] id_ex_rt;

  input wire [`NUM_REGISTERS_LOG2-1:0] ex_mem_rd0;
  input wire [`NUM_REGISTERS_LOG2-1:0] mem_wb_rd0;
  input wire ex_mem_reg_write0;
  input wire mem_wb_reg_write0;

  input wire [`NUM_REGISTERS_LOG2-1:0] ex_mem_rd1;
  input wire [`NUM_REGISTERS_LOG2-1:0] mem_wb_rd1;
  input wire ex_mem_reg_write1;
  input wire mem_wb_reg_write1;

  output reg [`FORWARD_BITS-1:0] forward_a0;
  output reg [`FORWARD_BITS-1:0] forward_b0;

  output reg [`FORWARD_BITS-1:0] forward_a1;
  output reg [`FORWARD_BITS-1:0] forward_b1;

  always @(*) begin

    if(ex_mem_reg_write0 && (id_ex_rs == ex_mem_rd0)) begin
      forward_a0 <= `FORWARD_EX_MEM;
    end else if(mem_wb_reg_write0 && (id_ex_rs == mem_wb_rd0)) begin
      forward_a0 <= `FORWARD_MEM_WB;
    end else begin
      forward_a0 <= `NO_FORWARD;
    end

    if(ex_mem_reg_write0 && (id_ex_rt == ex_mem_rd0)) begin
      forward_b0 <= `FORWARD_EX_MEM;
    end else if(mem_wb_reg_write0 && (id_ex_rt == mem_wb_rd0)) begin
      forward_b0 <= `FORWARD_MEM_WB;
    end else begin
      forward_b0 <= `NO_FORWARD;
    end

  end
endmodule
