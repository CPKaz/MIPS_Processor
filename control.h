#ifndef CONTROL_CLASS
#define CONTROL_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

// Control signals for the processor
struct control_t {
    bool reg_dest;           // 0 if rt, 1 if rd
    bool jump;               // 1 if jummp
    bool branch;             // 1 if branch
    bool mem_read;           // 1 if memory needs to be read
    bool mem_to_reg;         // 1 if memory needs to written to reg
    unsigned ALU_op : 2;     // 10 for R-type, 00 for LW/SW, 01 for BEQ/BNE, 11 for others
    bool mem_write;          // 1 if needs to be written to memory
    bool ALU_src;            // 0 if second operand is from reg_file, 1 if imm
    bool reg_write;          // 1 if need to write back to reg file
    
    void print() {      // Prints the generated contol signals
        cout << "REG_DEST: " << reg_dest << "\n";
        cout << "JUMP: " << jump << "\n";
        cout << "BRANCH: " << branch << "\n";
        cout << "MEM_READ: " << mem_read << "\n";
        cout << "MEM_TO_REG: " << mem_to_reg << "\n";
        cout << "ALU_OP: " << ALU_op << "\n";
        cout << "MEM_WRITE: " << mem_write << "\n";
        cout << "ALU_SRC: " << ALU_src << "\n";
        cout << "REG_WRITE: " << reg_write << "\n";
    }
    // TODO:
    // Decode instructions into control signals
    void decode(uint32_t instruction) {
    
    }
};




#endif
