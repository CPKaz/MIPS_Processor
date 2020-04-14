#ifndef ALU_CLASS
#define ALU_CLASS
#include <vector>
#include <cstdint>
#include <iostream>
class ALU {
    private:
        int ALU_control_inputs;
        int Second_op = 0;
    public:
        // TODO:
        // Generate the control inputs for the ALU
        void generate_control_inputs(int ALU_op, int funct, int opcode) {
	  //std::cout<<"OPCODE:"<<opcode<<std::endl;
	  //std::cout<<"FUNCTION:"<<funct<<std::endl;
        if (ALU_op==0b10){
            if (funct == 0x20 || funct == 0x21){ // add
                this->ALU_control_inputs = 2;
            }
            else if (funct == 0x24){ // and
                this->ALU_control_inputs = 0;
            }
            else if (funct == 0x8){ // jump register????
                this->ALU_control_inputs = 2;
            }
            else if (funct == 0x27){ // nor
	      this->ALU_control_inputs = 5; // five is now nor
            }
            else if (funct == 0x25){ // or
                this->ALU_control_inputs = 1;
            }
            else if (funct == 0x2A || funct == 0x2B || funct == 0xA || funct == 0xB){ // slt + sltu sli might not work not sure
                this->ALU_control_inputs = 7;
            }
            else if (funct == 0x25){ // or
                this->ALU_control_inputs = 1;
		//std::cout<<"OR"<<std::endl;
            }
            else if (funct == 0x00){
                this->ALU_control_inputs = 3; //shift left is now 3 alu op
            }
            else if (funct == 0x02){
                this->ALU_control_inputs = 4; //shift right is now 4 alu op
            }
            else if (funct == 0x22 || funct == 0x23){ // sub
                this->ALU_control_inputs = 6;
		//std::cout<<"SUBTRACTION"<<std::endl;
            }


        }
        else if (ALU_op==00){
            this->ALU_control_inputs = 2;

        }
        else {
            if (opcode == 0x8 || opcode == 0x9){ // add / andi
                this->ALU_control_inputs = 2;
            }
            else if (opcode == 0xc){ // andi
                this->ALU_control_inputs = 0;
            }
            else if (opcode == 0x4){ // beq
                this->ALU_control_inputs = 6;
		//std::cout<<"BRANCH"<<std::endl;
            }
            else if (opcode == 0x5){ // bne (not sub) 
                this->ALU_control_inputs = 8;
            }
            else if (opcode == 0x24 || opcode == 0x30 || opcode == 0x25){ // load byte unsigned
                this->ALU_control_inputs = 2;
            }
            else if (opcode == 0xf){ // load upper
                this->ALU_control_inputs = 9;
            }
            else if (opcode == 0xd){ // ori
                this->ALU_control_inputs = 1;
            }
            else if (opcode == 0xa || opcode == 0xb){ // slti & u
                this->ALU_control_inputs = 7;
            }
            else if (opcode == 0x28){ // store byte
                this->ALU_control_inputs = 2;
            }
            //store not required
            else if (opcode == 0x29){ // store half word
                this->ALU_control_inputs = 2;
            }
            else if (opcode == 0x2b){ // store word
                this->ALU_control_inputs = 2;
            }
        }
        }
        
        // TODO:
        // execute ALU operations, generate result, and set the zero control signal if necessary
        uint32_t execute(uint32_t operand_1, uint32_t operand_2, uint32_t &ALU_zero,int shamt) {
        int output=0;
        switch(this->ALU_control_inputs){
            case 0:
                output = operand_1 & operand_2; // need to for ori andi
		//std::cout<<operand_1<<"&"<<operand_2<<"="<<output<<std::endl;
                break;
            case 1:
                output = operand_1 | operand_2;
		//std::cout<<operand_1<<"|"<<operand_2<<"="<<output<<std::endl;
                break;
	    case 2:
	      output = operand_1 + operand_2;
	      //std::cout<<operand_1<<"+"<<operand_2<<"="<<output<<std::endl;
                break;
            case 3:
	      output = operand_2<<(shamt);
	      //std::cout<<operand_2<<"<<"<<shamt<<"="<<output<<std::endl;
		break;
            case 4:
	      output = operand_2>>(shamt);
	      //std::cout<<operand_2<<">>"<<shamt<<"="<<output<<std::endl;
		break;
            case 6:
                output = operand_1 - operand_2;
		//std::cout<<operand_1<<"-"<<operand_2<<"="<<output<<std::endl;
                break;
            case 7:
                if (operand_1 < operand_2){
                    output = 1;
                }
                else{
                    output = 0;
                }
		//std::cout<<operand_1<<"<"<<operand_2<<"="<<output<<std::endl;
                break;
            case 5:
	      output = ~(operand_1 | operand_2);
	      //std::cout<<operand_1<<"^"<<operand_2<<"="<<output<<std::endl;
                break;
	    case 8:
	      output = !(operand_1-operand_2);
	      //std::cout<<"bne:"<<operand_1<<"-"<<operand_2<<"="<<output<<std::endl;
	      break;
	    case 9:
	      //std::cout<<operand_2<<"<<16="<<output<<std::endl;
	      return operand_2<<(16);
	      break;
        }
	ALU_zero = !(output);
	//std::cout<<"ALU_zero:"<<ALU_zero<<std::endl;
	return output;
        }
            
};
#endif
