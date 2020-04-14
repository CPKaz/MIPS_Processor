#ifndef CONTROL_CLASS
#define CONTROL_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

// Control signals for the processor
struct control_t {
    bool reg_dest;           // 0 if rt, 1 if rd :: R-type vs I type
    bool jump;               // 1 if jump
    bool branch;             // 1 if branch
    bool mem_read;           // 1 if memory needs to be read :: load type
    bool mem_to_reg;         // 1 if memory needs to written to reg :: load type
    unsigned ALU_op : 2;     // 00 for LW/SW, 01 for BEQ/BNE, 10 for R-type, 11 for others
    bool mem_write;          // 1 if needs to be written to memory :: write type
    bool ALU_src;            // 0 if second operand is from reg_file, 1 if imm :: I-type
    bool reg_write;          // 1 if need to write back to reg file:: R-type & load type
    
    bool halfword;
    bool byte;
    bool u_extend;
    bool is_instruction;


    void print() {      // Prints the generated contol signals
        std::cout << "REG_DEST: " << reg_dest << "\n";
        std::cout << "JUMP: " << jump << "\n";
        std::cout << "BRANCH: " << branch << "\n";
        std::cout << "MEM_READ: " << mem_read << "\n";
        std::cout << "MEM_TO_REG: " << mem_to_reg << "\n";
        std::cout << "ALU_OP: " << ALU_op << "\n";
        std::cout << "MEM_WRITE: " << mem_write << "\n";
        std::cout << "ALU_SRC: " << ALU_src << "\n";
        std::cout << "REG_WRITE: " << reg_write << "\n";
    }
    // TODO:
    // Decode instructions into control signals
    void decode(uint32_t instruction) {
      this->reg_dest = 1;
      this->reg_write = 1;
      this->branch = 0;
      this->mem_read = 0;
      this->mem_to_reg = 0;
      this->ALU_op = 0b10;
      this->mem_write = 0;
      this->ALU_src = 0;
      this->halfword = 0;
      this->byte = 0;
      this->is_instruction = 0;
        uint32_t opcode = instruction>>26;
	if (instruction != 0){
        this->is_instruction = 1;
	  if (opcode == 0x00){ // R-type
            this->reg_dest = 1;
            this->reg_write = 1;
            this->branch = 0;
            this->mem_read = 0;
            this->mem_to_reg = 0;
            this->ALU_op = 0b10;
            this->mem_write = 0;
            this->ALU_src = 0;
            
            uint32_t funct_mask = 0x0000003f;
            uint32_t funct = instruction & funct_mask;
            if(funct == 0x08) { this->jump = 1; // jr
	      this->reg_dest = 0;
	      this->reg_write = 0;
	      this->ALU_op=00;
            } else { this->jump = 0; }
        }

        else { // I-type
            this->reg_dest = 0;
            this->jump = 0;
            this->branch = 0;
	    this->u_extend = 0;
	    if (opcode == 0xd || opcode == 0xc){
	      this->u_extend = 1;
	    }

            if((opcode >= 0x04 && opcode <= 0x0d) || opcode == 0xf) { // branch/other
                this->mem_read = 0;
                this->mem_to_reg = 0;
                this->mem_write = 0;
                if(opcode == 0x04 || opcode == 0x05) { // branch
                    this->reg_write = 0;
                    this->branch = 1;
                    this->ALU_op = 01;
                    this->ALU_src = 0;
                } else {
                    this->reg_write = 1;
                    this->ALU_op = 0b11;
                    this->ALU_src = 1;
                }
            }
            else { // load/store
                this->ALU_op = 00;
                this->ALU_src = 1;
                if(opcode <= 0x25 || opcode == 0x30) { // load
                    this->reg_write = 1;
                    this->mem_read = 1;
                    this->mem_to_reg = 1;
                    this->mem_write = 0;
		    if(opcode == 0x25){
		      this->halfword= 1;
		    }
		    else if(opcode == 0x24){
		      this->byte = 1;
		    }
                }
                else { // store
                    this->reg_write = 0;
                    this->mem_read = 0;
                    this->mem_to_reg = 0;
                    this->mem_write = 1;
		    if (opcode == 0x29){
		      this->halfword = 1;
		    }
		    else if(opcode == 0x28){
		      this->byte = 1;
		    }
                }
            }
        }
	}
    }
};




#endif
