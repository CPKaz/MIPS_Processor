#ifndef STATE
#define STATE
#include <vector>
#include <cstdint>
#include <iostream>
#include "control.h"
//
// Pipeline registers implementation
//
// TODO:
// IFID Pipeline register, only contains instruction and pc + 1

  control_t z = {.reg_dest = 0, 
                        .jump = 0,
                        .branch = 0,
                        .mem_read = 0,
                        .mem_to_reg = 0,
                        .ALU_op = 0,
                        .mem_write = 0,
                        .ALU_src = 0,
			 .reg_write = 0,
			 .halfword = 0,
			 .byte = 0,
                         .u_extend = 0,
                        .is_instruction = 0};

struct IFID {
IFID() : instruction(0), pc(0) {}
  uint32_t instruction;
  uint32_t pc;
};

// TODO:
// IDEX Pipeline register
struct IDEX {
IDEX() : rs_val(0), rt_val(0), imm(0), rs(0), rt(0), rd(0), pc(0), function(0), opcode(0), shamt(0), control(z) {}
    control_t control;
  uint32_t rs_val;
  uint32_t rt_val;
  uint32_t imm;
  uint32_t rs;
  uint32_t rt;
  uint32_t rd;
  uint32_t pc;
  uint32_t function;
  uint32_t opcode;
  uint32_t shamt;
};

// TODO:
// EXMEM Pipeline register
struct EXMEM {
EXMEM() : rs_val(0), rt_val(0), imm(0), rs(0), rt(0), rd(0), pc(0), ALU_result(0), ALU_zero(0), result_reg(0), control(z) {}
    control_t control;
  uint32_t rs_val;
  uint32_t rt_val;
  uint32_t imm;
  uint32_t rs;
  uint32_t rt;
  uint32_t rd;
  uint32_t pc;
  uint32_t ALU_result;
  uint32_t  ALU_zero;
  uint32_t result_reg;
  
};

// TODO:
// MEMWB Pipwline register
struct MEMWB {
MEMWB() : mem_data(0), ALU_result(0), ALU_zero(0), result_reg(0), control(z) {}
  uint32_t result_reg;
    control_t control;
  uint32_t ALU_result;
  uint32_t  ALU_zero;
  uint32_t mem_data;
};

struct state_t {
  uint32_t pc;
  IFID ifid1;
  IFID ifid2;
  IDEX idex1;
  IDEX idex2;
  EXMEM exmem1;
  EXMEM exmem2;
  MEMWB memwb1;
  MEMWB memwb2;

};


#endif
