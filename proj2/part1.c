#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"

void decode_instruction(Instruction instruction) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0 (SPECIAL)
            switch(instruction.rtype.funct) {
                case 0xc: // funct == 0xc (SYSCALL)
                    printf("syscall\n");
                    break;
                case 0x24: // funct == 0x24 (AND)
                    printf("and\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x00: // funct == 0x00 (sll)
                    printf("sll\t$%d,$%d,%u\n", instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                case 0x02: // funct == 0x02 (srl)
                    printf("srl\t$%d,$%d,%u\n", instruction.rtype.rd, instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                case 0x03: // funct == 0x03 (sra)
                    printf("sra\t$%d,$%d,$%u\n", instruction.rtype.rd, instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                case 0x08: // funct == 0x08 (jr)  
                    printf("jr\t$%d\n", instruction.rtype.rs);
                    break;
                case 0x09: // funct == 0x09 (jalr) 
                    printf("jalr\t$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs);
                    break;
                case 0x10: // funct == 0x10 (mfhi)
                    printf("mfhi\t$%d\n", instruction.rtype.rd);
                    break;
                case 0x12: // funct == 0x12 (mflo)
                    printf("mflo\t$%d\n", instruction.rtype.rd);
                    break;
                case 0x18: // funct == 0x18 (mult)
                    printf("mult\t$%d,$%d\n", instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x19: // funct == 0x19 (multu)
                    printf("multu\t$%d,$%d\n", instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x21: // funct == 0x21 (addu)
                    printf("addu\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x23: // funct == 0x23 (subu)
                    printf("subu\t$%d,$%d,$%d\n", instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x25: // funct == 0x25 (or)
                    printf("or\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x26: // funct == 0x26 (xor)
                    printf("xor\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x27: // funct == 0x27 (nor)
                    printf("nor\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x2a: // funct == 0x2a (slt)
                    printf("slt\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                case 0x2b: // funct == 0x2b (sltu)
                    printf("sltu\t$%d,$%d,$%d\n",instruction.rtype.rd, instruction.rtype.rs, instruction.rtype.rt);
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: illegal function: %08x\n",__FUNCTION__,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0xd: // opcode == 0xd (ORI)
            printf("ori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
        case 0x04: // opcode == 0x04 (beq)
            printf("beq\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, instruction.itype.imm*4);
            break;
        case 0x05: // opcode == 0x05 (bne)
            printf("bne\t$%d,$%d,%d\n", instruction.itype.rs, instruction.itype.rt, instruction.itype.imm*4);
            break;
        case 0x09: //addiu
            printf("addiu\t$%d,$%d,%d\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
            break;
        case 0x0a: //slti
            printf("slti\t$%d,$%d,$%d\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
            break;
        case 0x0b: //sltiu
            printf("sltiu\t$%d,$%d,$%d\n",instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
            break;
        case 0x0c: //andi
            printf("andi\t$%d,$%d,0x%d\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
        case 0x0e: //xori
            printf("xori\t$%d,$%d,0x%d\n", instruction.itype.rt, instruction.itype.rs, instruction.itype.imm);
            break;
        case 0x0f: //lui
            printf("lui\t$%d,0x%x\n", instruction.itype.rt, instruction.itype.imm);
            break;
        case 0x20: //lb
            printf("lb\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x21: //lh
            printf("lh\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x23: //lw
            printf("lw\t$%d,%d($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x24: //lbu
            printf("lbu\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x25: //lhu
            printf("lhu\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x28: //sb
            printf("sb\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x29: //sh
            printf("sh\t$%d,%d,($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x2b: //sw
            printf("sw\t$%d,%d($%d)\n", instruction.itype.rt, instruction.itype.imm, instruction.itype.rs);
            break;
        case 0x2: // opcode == 0x2 (J)
            printf("j\t0x%x\n",instruction.jtype.addr*4);
            break;
        case 0x3: // opcode =- 0x3 (jal)
            printf("jal\t0x%x\n", instruction.jtype.addr*4);
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: illegal instruction: %08x\n",__FUNCTION__,instruction.bits);
            exit(-1);
            break;
    }
}
