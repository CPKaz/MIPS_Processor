#include <cstdint>
#include <iostream>
#include "memory.h"
#include "reg_file.h"
#include "ALU.h"
#include "control.h"
#include "state.h"
#include <map>
#include <vector>
using namespace std;

void processor_main_loop(Registers &reg_file, Memory &memory, uint32_t end_pc) {

    // Initialize ALU
    ALU alu;
    //Initialize Control
    control_t control = {.reg_dest = 0, 
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
                         .u_extend = 0 };
    
    uint32_t num_cycles = 0;
    uint32_t num_instrs = 0;

    while (reg_file.pc != end_pc) {

        // fetch
        uint32_t instruction;
        memory.access(reg_file.pc, instruction, 0, 1, 0);
        std::cout << "\nPC: 0x" << std::hex << reg_file.pc << std::dec << "\n";
        // increment pc
        reg_file.pc += 4;
        
        // TODO: fill in the function argument
        // decode into contol signals
        control.decode(instruction);
        control.print(); // used for autograding

        
        // TODO: fill in the function argument
        // Read from reg file

	uint32_t rs_val = 0;

        uint32_t rt_val = 0;

        uint32_t rs = (instruction & 0x03e00000) >> 21;

        uint32_t rt = (instruction & 0x001f0000) >> 16;

        uint32_t rd = (instruction & 0x0000f800) >> 11;

        uint32_t imm = (instruction & 0x0000ffff);

        uint32_t shamt = (instruction & 0x000007c0) >> 6;

	u_int32_t opcode = (instruction & 0xfc000000)>>26;
        u_int32_t function = (instruction & 0x3f);

	uint32_t imm_sign_bit = 0x00008000;

        uint32_t write_data = 0;
        uint32_t zero = 0;


        if ((imm & imm_sign_bit) && !(control.u_extend)){ //negative sign extend
            imm = (imm | 0xffff0000);
        }
        //if (control.reg_dest || control.branch){
        reg_file.access(rs, rt, rs_val, rt_val, 0, 0, 0);
	    // }
        //else if (control.ALU_src || control.jump){
        //    reg_file.access(rs, 0, rs_val, zero, 0, 0, 0);
        //}
        //else if (control.branch){
	//  reg_file.access(rs, rt, rs_val, rt_val, 0, 0, 0);
	//}
        
        // TODO: fill in the function argument
        // Execution 

        alu.generate_control_inputs(control.ALU_op, function, opcode);
        
        
        // TODO: fill in the function argument
        uint32_t ALU_zero=0;
        uint32_t alu_result=0;
        uint32_t result_reg=0;
	std::cout<<"OPCODE:"<<opcode<<std::endl;
	std::cout<<"FUNCTION:"<<function<<std::endl;
        if (control.ALU_src){
	  alu_result = alu.execute(rs_val, imm, ALU_zero, shamt);
            result_reg = rt;
        }
        else {
	  alu_result = alu.execute(rs_val, rt_val, ALU_zero, shamt);
            result_reg = rd;
        }
	cout<<"alu_result:"<<alu_result<<endl;
	cout<<"result_reg:"<<result_reg<<endl;

        
        // Memory
        // TODO: fill in the function argument
        uint32_t mem_data=0;

	std::cout<<"rs:"<<rs<<":"<<rs_val<<std::endl;
	std::cout<<"rt:"<<rt<<":"<<rt_val<<std::endl;
	std::cout<<"imm:"<<imm<<std::endl;
	//std::cout<<"ALU:"<<alu_result<<endl;

        
        if (control.mem_read || (control.mem_write && control.halfword)||(control.mem_write && control.byte)){
            memory.access(alu_result, mem_data, 0, 1, 0);
	    //cout<<"loading"<<endl;
	    //cout<<"rs:"<<rs<<":"<<rs_val<<endl;
	    //cout<<"rt:"<<rt<<":"<<rt_val<<endl;
	    //cout<<"mem_data:"<<mem_data<<endl;
	    //cout<<"ALU:"<<alu_result<<endl;
	    if (control.mem_write){
	      if(control.halfword){
		rt_val = ((mem_data & 0xffff0000) | (rt_val & 0x0000ffff));//bitwise or result reg
		// cout<<"upper_data:"<<(mem_data & 0xffff0000)<<"lower:"<<(rt_val & 0x0000ffff)<<"combo:"<<rt_val<<endl;
	      }
	      else if(control.byte){
		rt_val = (mem_data & 0xffffff00) | (rt_val & 0x000000ff);
		}
	    }
	    else if (control.halfword){
	      mem_data = (mem_data & 0x0000ffff);
	      //cout<<"half-word:"<<mem_data<<endl;
	    }
	    else if (control.byte){
	      mem_data = (mem_data & 0x000000ff);
	    }
        }
	//cout<<"mem_data_update:"<<mem_data<<endl;
        if (control.mem_write){
            memory.access(alu_result, rt_val, rt_val, 0, 1);
	    //cout<<"storing:"<<rt_val<<endl;
	    //cout<<"rs:"<<rs<<":"<<rs_val<<endl;
	    //cout<<"rt:"<<rt<<":"<<rt_val<<endl;
	    //cout<<"mem_data:"<<mem_data<<endl;
	    //cout<<"ALU:"<<alu_result<<endl;
        } 
        
        // Write Back
        // TODO: fill in the function argument
	uint32_t read_data= 0;
	if(control.mem_to_reg){
	  reg_file.access(0, 0, zero, zero, result_reg, 1, mem_data);
	  //cout<<"reg_write:"<<mem_data<<endl;
	  
        }
	else if(control.reg_write){
	  reg_file.access(0, 0, zero, zero, result_reg, 1, alu_result);
	  
        }
        //cout<<"ALU_RESULT:"<<alu_result<<endl;
        // TODO: Update PC 
        if (control.branch && ALU_zero){
	  //std::cout<<"branch_taken"<<std::endl;
	  reg_file.pc = reg_file.pc + 4*imm;
	  //std::cout<<"imm"<<imm<<std::endl;
	  //std::cout<<"address_after_branch"<<reg_file.pc<<std::endl;
        }
	if(control.jump){
	  reg_file.pc = rs_val;
	}


        std::cout << "CYCLE" << num_cycles << "\n";
        reg_file.print(); // used for automated testing
        num_cycles++;
        num_instrs++;

    }

    std::cout << "CPI = " << (double)num_cycles/(double)num_instrs << "\n";

}
void processor_pipeline_loop(Registers &reg_file, Memory &memory, uint32_t end_pc){


  float num_cycles = 0;

   float num_instrs = 0;

   float bonus_cycles = 0;

   state_t state;
   
   
    control_t nop = {.reg_dest = 0, 
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

  while (reg_file.pc != end_pc+16) {

    //___________________________________________________________________________________________
	//___________________________________MEMWB____________________________________________________
	//____________________________________________________________________________________________
    bool rt_overwrite = 0;
    bool rs_overwrite = 0;
    state.exmem2 = state.exmem1;
    state.memwb2 = state.memwb1;
    if ((state.exmem2.control.mem_read && state.idex1.control.is_instruction && (state.exmem2.rt == state.idex1.rs || (state.exmem2.rt == state.idex1.rt && state.idex1.rd)))){

        reg_file.pc -= 4;
        state.idex2.control = nop;
	num_instrs--;
        }
    else {
    state.ifid2 = state.ifid1;
    state.idex2 = state.idex1;
    }
    if (state.exmem2.result_reg == state.idex2.rs && state.exmem2.control.reg_write){ //no mem read?
      //cout<<"Forwarding:"<<state.exmem2.result_reg<<":"<<state.exmem2.ALU_result<<endl;
      //cout<<"Overwritten:"<<state.idex2.rs<<":"<<state.idex2.rs_val<<endl;
        state.idex2.rs_val = state.exmem2.ALU_result; //
        rs_overwrite = 1;
    }
    if (state.exmem2.result_reg == state.idex2.rt && state.exmem2.control.reg_write){
      //cout<<"Forwarding:"<<state.exmem2.result_reg<<":"<<state.exmem2.ALU_result<<endl;
      //cout<<"Overwritten:"<<state.idex2.rt<<":"<<state.idex2.rt_val<<endl;
        state.idex2.rt_val = state.exmem2.ALU_result;
        rt_overwrite=1;
    }
    
    
    uint32_t zero = 0;
    uint32_t read_data= 0;
	if(state.memwb2.control.mem_to_reg){
	  reg_file.access(0, 0, zero, zero, state.memwb2.result_reg, 1, state.memwb2.mem_data);
	  if (state.memwb2.result_reg == state.idex2.rs && !(rs_overwrite)){ //other registers
        state.idex2.rs_val = state.memwb2.mem_data;
    }
    if (state.memwb2.result_reg == state.idex2.rt && !(rt_overwrite)){
        state.idex2.rt_val = state.memwb2.mem_data;
    }
    if (state.memwb2.result_reg == state.exmem2.rt && !(rt_overwrite)){
        state.exmem2.rt_val = state.memwb2.mem_data;
    }
    if (state.memwb2.result_reg == state.exmem2.rs && !(rs_overwrite)){
        state.exmem2.rs_val = state.memwb2.mem_data;
    }
        }
	else if(state.memwb2.control.reg_write){
	  reg_file.access(0, 0, zero, zero, state.memwb2.result_reg, 1, state.memwb2.ALU_result);
	  //cout<<"Phase 5: writing "<<state.memwb2.ALU_result<<"to register "<<state.memwb2.result_reg<<endl;
    if (state.memwb2.result_reg == state.idex2.rs && !(rs_overwrite)){
        state.idex2.rs_val = state.memwb2.ALU_result;
    }
    if (state.memwb2.result_reg == state.idex2.rt && !(rt_overwrite)){
        state.idex2.rt_val = state.memwb2.ALU_result;
    }
    if (state.memwb2.result_reg == state.exmem2.rs && !(rs_overwrite)){
        state.exmem2.rs_val = state.memwb2.ALU_result;
    }
    if (state.memwb2.result_reg == state.exmem2.rt && !(rt_overwrite)){
        state.exmem2.rt_val = state.memwb2.ALU_result;
    }
        }   
    
    ALU alu;
    //Initialize Control
    control_t control = {.reg_dest = 0, 
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
    
    uint32_t instruction;

        memory.access(reg_file.pc, instruction, 0, 1, 0);

               if (reg_file.pc < end_pc){
                    std::cout << "\nPC: 0x" << std::hex << reg_file.pc << std::dec << "\n";
                    num_instrs++;
                    }
                else{
                    cout<<"\n";
                }

        reg_file.pc += 4;
	//cout<<"Phase 1: Fetching "<< instruction <<endl;
	

	state.ifid1.instruction  = instruction;
	state.ifid1.pc = reg_file.pc;
	//_____________________________________________________________________________________________
	//____________________________________IFID_____________________________________________________
	//_____________________________________________________________________________________________
	//DO NOT USE IFID1
    control.decode(state.ifid2.instruction);
    //control.print();
    
	instruction = state.ifid2.instruction;
	uint32_t opcode = (instruction & 0xfc000000)>>26;
	uint32_t rs = (instruction & 0x03e00000) >> 21;
	uint32_t rt = (instruction & 0x001f0000) >> 16;
	uint32_t rd = (instruction & 0x0000f800) >> 11;
	uint32_t shamt = (instruction & 0x000007c0) >> 6;
	uint32_t function = (instruction & 0x3f);
	uint32_t imm = (instruction & 0x0000ffff);
	uint32_t pc = state.ifid2.pc;


	uint32_t rs_val = 0;
        uint32_t rt_val = 0;
	
        //cout<<"rs:"<<rs<<":"<<rs_val<<endl;
	//cout<<"rt:"<<rt<<":"<<rt_val<<endl;
	reg_file.access(rs, rt, rs_val, rt_val, 0, 0, 0);
	
	


	if ((imm & 0x0000800) && !(control.u_extend)){ //should I put the control signals in the pipeline?
	  imm  = (imm | 0xffff0000);
        }
	//cout<<"Phase 2: Decoding"<<endl;
    //cout<<"rs:"<<rs<<":"<<rs_val<<endl;
    //cout<<"rt:"<<rt<<":"<<rt_val<<endl;
    //cout<<"imm:"<<imm<<endl;
    
	state.idex1.rs_val = rs_val;
	state.idex1.rt_val = rt_val;
	state.idex1.shamt = shamt;
	state.idex1.imm = imm;
	state.idex1.rt = rt;
	state.idex1.rs = rs;
    state.idex1.rd = rd;
	state.idex1.pc = pc - 4;
	state.idex1.control = control;
	state.idex1.opcode = opcode;
	state.idex1.function = function;
	//_____________________________________________________________________________________________
	//___________________________________IDEX______________________________________________________
	//_____________________________________________________________________________________________

	uint32_t ALU_op = state.idex2.control.ALU_op;
	
	uint32_t  ALU_zero=0;
        uint32_t ALU_result=0;
        uint32_t result_reg=0;
	//std::cout<<"OPCODE:"<<opcode<<std::endl;
	//std::cout<<"FUNCTION:"<<function<<std::endl;

	alu.generate_control_inputs(ALU_op, state.idex2.function, state.idex2.opcode); //fix this
	
        if (state.idex2.control.ALU_src){
	 ALU_result = alu.execute(state.idex2.rs_val, state.idex2.imm, ALU_zero, state.idex2.shamt);
            result_reg = state.idex2.rt;
            //cout<<"immediate"<<endl;
        }
        else {
	  ALU_result = alu.execute(state.idex2.rs_val, state.idex2.rt_val, ALU_zero, state.idex2.shamt);
            result_reg = state.idex2.rd;
	}
    
    
    if (state.idex2.control.branch && ALU_zero){
      num_instrs -= 2;
            reg_file.pc = state.idex2.pc + 4*state.idex2.imm + 4;
            state.idex1.control = nop;
            state.idex1.rs_val = 0;
	state.idex1.rt_val = 0;
	state.idex1.imm = 0;
	state.idex1.rt = 0;
	state.idex1.rs = 0;
    state.idex1.rd = 0;
	state.idex1.opcode = 0;
	state.idex1.function = 0;
    state.ifid1.instruction = 0;
    state.ifid2 = state.ifid1;
    state.idex2 = state.idex1;
        }
	//cout<<"Stage 3 Executing:"<<endl;
    //cout<<"ALU:"<<ALU_result<<endl;
    //cout<<"rs:"<<state.idex2.rs<<":"<<state.idex2.rs_val<<endl;
    //cout<<"rt:"<<state.idex2.rt<<":"<<state.idex2.rt_val<<endl;
    //cout<<"imm:"<<state.idex2.imm<<endl;
    
    
     state.exmem1.result_reg = result_reg;
     state.exmem1.ALU_result = ALU_result;
     state.exmem1.ALU_zero = ALU_zero;
     state.exmem1.control = state.idex2.control;
     state.exmem1.rt_val = state.idex2.rt_val;
     state.exmem1.rt = state.idex2.rt;
     state.exmem1.rs_val = state.idex2.rs_val;
     state.exmem1.rs = state.idex2.rs;

     	//_____________________________________________________________________________________________
	//___________________________________EXMEM______________________________________________________
	//_____________________________________________________________________________________________

        uint32_t mem_data=0;
        if (!(state.exmem2.control.mem_read || state.exmem2.control.mem_write)){
	  //cout<<"No memory access:"<<endl;
            //cout<<"rs:"<<state.exmem2.rs<<":"<<state.exmem2.rs_val<<endl;
            //cout<<"rt:"<<state.exmem2.rt<<":"<<state.exmem2.rt_val<<endl;
        }
        
        if (state.exmem2.control.mem_read ||
	    (state.exmem2.control.mem_write && state.exmem2.control.halfword)||
	    (state.exmem2.control.mem_write && state.exmem2.control.byte)){
            memory.access(state.exmem2.ALU_result, mem_data, 0, 1, 0);
	    //cout<<"loading"<<endl;
	    //cout<<"rs:"<<state.exmem2.rs<<":"<<state.exmem2.rs_val<<endl;
	    //cout<<"rt (value to be stored):"<<state.exmem2.rt<<":"<<state.exmem2.rt_val<<endl;
	    //cout<<"mem_data:"<<mem_data<<endl;
	    //cout<<"ALU:"<<state.exmem2.ALU_result<<endl;
	    if (state.exmem2.control.mem_write){
	      if(state.exmem2.control.halfword){
		state.exmem2.rt_val = (mem_data & 0xffff0000) | (state.exmem2.rt_val & 0x0000ffff);//bitwise or result reg
	      }
	      else if(control.byte){
		state.exmem2.rt_val = (mem_data & 0xffffff00) | (state.exmem2.rt_val & 0x000000ff);
		}
	    }
	    else if (state.exmem2.control.halfword){
	      mem_data = (mem_data & 0x0000ffff);
	    }
	    else if (state.exmem2.control.byte){
	      mem_data = (mem_data & 0x000000ff);
	    }
        }
	//std::cout<<"mem_data:"<<mem_data<<std::endl;
	//std::cout<<"rt_val_2:"<<rt_val<<std::endl;
        if (state.exmem2.control.mem_write){
            memory.access(state.exmem2.ALU_result, state.exmem2.rt_val, state.exmem2.rt_val, 0, 1);
	    //cout<<"storing"<<endl;
	    //cout<<"rs:"<<state.exmem2.rs<<":"<<state.exmem2.rs_val<<endl;
	    //cout<<"rt:"<<state.exmem2.rt<<":"<<state.exmem2.rt_val<<endl;
	    //cout<<"mem_data:"<<mem_data<<endl;
	    //cout<<"ALU:"<<state.exmem2.ALU_result<<endl;
        }
	
	state.memwb1.mem_data = mem_data;
	state.memwb1.control = state.exmem2.control;
	state.memwb1.result_reg = state.exmem2.result_reg;
	state.memwb1.ALU_result = state.exmem2.ALU_result;
	
	std::cout << "CYCLE" << num_cycles << "\n";
        reg_file.print(); // used for automated testing

        num_cycles++;

  }
  cout<<"CPI = "<<(num_cycles+bonus_cycles)/num_instrs<<endl;
  //cout<<num_cycles<<endl;
  //cout<<num_instrs;
}
