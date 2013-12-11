#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                case 0x00: // sll
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x02: // srl
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x03: // sra
                    processor->R[instruction.rtype.rd] = (sWord) processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x08: // jr
                    processor->PC = processor->R[instruction.rtype.rs];
                    break;
                case 0x09: // jalr
                    {sWord tmp = processor->PC + 4;
                    processor->PC = processor->R[instruction.rtype.rs];
                    processor->R[instruction.rtype.rd] = tmp;
                    break;
                    };
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                case 0x10: // mfhi
                    processor->R[instruction.rtype.rd] = processor->RHI;
                    processor->PC += 4;
                    break;
                case 0x12: // mflo
                    processor->R[instruction.rtype.rd] = processor->RLO;
                    processor->PC += 4;
                    break;
                case 0x18: // mult
                    {sWord tmp = (sWord)processor->R[instruction.rtype.rs] * (sWord)processor->R[instruction.rtype.rt];
                    processor->RLO = tmp & 0x00000000ffffffff;  // 32 bits
                    processor->RHI = tmp & 0xffffffff00000000;  // 32 16 bits
                    processor->PC += 4;
                    break;
                    };
                case 0x19: // multu Unfinished
                    {sWord tmp = processor->R[instruction.rtype.rs] * processor->R[instruction.rtype.rt];
                    processor->RLO = tmp & 0x00000000ffffffff;  // lower 16 bits
                    processor->RHI = tmp & 0xffffffff00000000;  // upper 16 bits
                    processor->PC += 4;
                    break;
                    };
                case 0x21: // addu Unfinished
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x23: // subu
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x25: // or
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    break;
                case 0x26: // xor
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x27: // nor
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                case 0x2a: // slt
                    processor->R[instruction.rtype.rd] = ((sWord) processor->R[instruction.rtype.rs] < (sWord) processor->R[instruction.rtype.rt]); 
                    processor->PC += 4;
                    break;
                case 0x2b: // sltu
                    if (processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt]) {
                        processor->R[instruction.rtype.rd] = 1;
                    } else {
                        processor->R[instruction.rtype.rd] = 0;
                    }
                    processor->PC += 4;
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x03: // jal
            processor->R[31] = (processor->PC + 4);
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x04: // beq
            if (processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]){
                processor->PC += 4 + (sHalf) (instruction.itype.imm << 2);
            } else {
                processor->PC +=4;
            }
            break;
        case 0x05: // bne
            if (processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]) {
                processor->PC += 4 + (sHalf) (instruction.itype.imm << 2);
            } else {
                processor->PC += 4;
            }
            break;
        case 0x09: // addiu
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sHalf) instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0a: // slti
            processor->R[instruction.itype.rt] = (sWord) processor->R[instruction.itype.rs] < (sHalf) instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0b: // slitu
            processor->R[instruction.itype.rt] = (processor->R[instruction.itype.rs] < (sHalf) instruction.itype.imm);
            processor->PC += 4;
            break;
        case 0x0c: // andi 
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0e: // xori
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x0f: // lui
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
            break;
        case 0x20: // lb FIX UNFINISHED FOLLOWING REQUIRE FUTURE FUNCTIONS
            processor->R[instruction.itype.rt] = (sByte)load(memory, processor->R[instruction.itype.rs] + (sHalf) (instruction.itype.imm), LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x21: // lh FIX UNFINISHED
            processor->R[instruction.itype.rt] = (sHalf)load(memory, processor->R[instruction.itype.rs] + (sHalf) (instruction.itype.imm), LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x23: // lw FIX UNFINISHED
            processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sHalf) (instruction.itype.imm), LENGTH_WORD);
            processor->PC += 4;
            break;
        case 0x24: // lbu UNFINISHED FIX
            processor->R[instruction.itype.rt] = (load(memory, processor->R[instruction.itype.rs] + (sHalf) (instruction.itype.imm), LENGTH_BYTE));
            processor->PC += 4;
            break;
        case 0x25: // lhu FIX UNFINISHED
            processor->R[instruction.itype.rt] = (load(memory, processor->R[instruction.itype.rs] + (sHalf) (instruction.itype.imm), LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x28: // sb FIX UNFINISHEDD REQUIRES FUNCTION STOREMEM
            store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29: // sh FIX UNFINISHED
            store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x2b: // sw FIX UNFINISHED
            store(memory, processor->R[instruction.itype.rs] + (sHalf)(instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}

// takes the address of a memory access and the size of the value being returned 
// and returns 1 if it is a valid access or 0 if not
int check(Address address,Alignment alignment) {

    /* YOUR CODE HERE */
    if (address < 1 || address > MEMORY_SPACE) {
        return 0;
    } 
    if (address % alignment != 0) {
        return 0;
    } else{
        return 1;
    }
}
// takes an address, a size, and a value and stores the first -size- bytes
// of the given value at the given address.
//address
//alignment = byte size
//value
void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    /* YOUR CODE HERE */
    int i;
    for (i = 0; i < alignment; i++){
        *(i + memory + address) = value & 0x000000ff;
        value = value >> 8;
    } 
}
// takes an address and a size and 
// returns the next -size- bytes starting at the given address
// address
// alignment
Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    if (alignment == LENGTH_BYTE) {
        return *(Byte*)(memory + address);
    }
    else if (alignment == LENGTH_HALF_WORD) {
        return *(Half*)(memory + address);
    }else{
        return *(Word*)(memory + address);
    }
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    
}

