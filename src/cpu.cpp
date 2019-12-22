//
// Created by marten on 17-12-19.
//

#include "cpu.h"
#include "chip8.h"

void CPU::initialize(Chip8 *chip8) {
    chip8_ = chip8;
    pc      = 0x200;    // Program counter starts at 0x200
    opcode  = 0;        // Reset current opcode
    I       = 0;        // Reset index register
    sp      = 0;        // Reset stack pointer

    for(auto reg : V)
        reg = 0;

//    for(int i = 0; i < 80; i++)
//        memory[i] = chip8_fontset[i];
    delay_timer = 0;
    sound_timer = 0;
}


#include <iostream>
void CPU::emulateCycle() {
    /* Store a byte from memory into the 2 byte opcode variable.
       Shift it so it is stored in the high byte.
       Store another byte from memory in to the lower byte */
    opcode =  chip8_->memory[pc] << 8 |  chip8_->memory[pc + 1];
    pc += 2;

    // $ = Value of register
    // # = Immediate value
    // Compare the first character
    switch(opcode & 0xF000){
        case 0xA000:    //
            printf("SETI %d",(opcode & 0x0FFF));
        break;
        case 0xC000:    //
            printf("RAND $%d",(opcode & 0x0F00),", #",(opcode & 0x00FF));
        break;
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0:   // 00E0 Clear screen
                    printf("CLS");
                break;
                case 0x00EE:   // 00EE Return from subroutine
                    printf("RET");
                break;
                default:        // 0xxx
                    printf("CALL.RCA_1802 ",(opcode & 0x0FFF));
                break;
            }
        break;
        case 0x1000:   // 1xxx Jump to address $xxx
            printf("JMP $0x%X\n",(opcode & 0x0FFF));
        break;
        case 0x2000:   // 2xxx Call subroutine $xxx
            printf("CALL $0x%X\n",(opcode & 0x0FFF));
        break;
        case 0xB000:   // Bxxx Jump to address $(xxx + V0)
            printf("JMP $0x%X(V0)\n",(opcode & 0x0FFF));
        break;
        case 0x3000:   // 3xnn Skip if Equal (Comparing immediate value to a register, if(Vx == nn))
            printf("SKIP.EQ $0x%X, #0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
        break;
        case 0x4000:   // Skip if Not Equal (Comparing immediate value to a register, if(Vx == nn))
            printf("SKIP.NE $0x%X, #0x%X\n",(opcode & 0x0F00), (opcode & 0x00FF));
        break;
        case 0x5000:   // Skip if Equal (Comparing registers, if(Vx == Vy))
            printf("SKIP.EQ $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0));
        break;
        case 0x6000:    // 6xnn Assign immediate value Vx = nn
            printf("MOV $0x%X, #0x%X\n", (opcode & 0x0F00), (opcode & 0x0F00));
        break;
        case 0x7000:    // 7xnn Add immediate value Vx += nn
            printf("ADD $0x%X, #0x%X\n", (opcode & 0x0F00), (opcode & 0x0F00));
        break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:   // 8xx0 Move register to register (Vx = Vy)
                    printf("MOV $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0001:   // 8xy1 OR   x |= y
                    printf("OR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0002:   // 8xy2 AND  x &= y
                    printf("AND $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0003:   // 8xy3 XOR x ^= y
                    printf("XOR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0004:   // 8xy4 Add x += y
                    printf("ADD $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0005:   // 8xy5 Subtract x -= y
                    printf("SUB $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    break;
                case 0x0006:   // 8xy6 Shift right x >>= 1
                    printf("SHR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    //printf("SHL $",(opcode & 0x0F00));
                    break;
                case 0x0007:   // 8xy7 Subtract backwards x = y - x
                    printf("SUBB $0x%X\n",(opcode & 0x0F00));
                    break;
                case 0x000E:   // 8xyE Shift left x <<= y
                    printf("SHL $0x%X, $0x%X\n",(opcode & 0x0F00), (opcode & 0x00FF));
                    //printf("SHL $",(opcode & 0x0F00));
                    break;
            }
        break;
        case 0x9000:   // Skip if Not Equal (Comparing registers, if(Vx == Vy))
            printf("SKIP.NE $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0));
        break;
        case 0xD000:   // Dxyn Display value n in (x,y)
            printf("DISP $0x%X, $0x%X, #0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0),(opcode & 0x000F));
        break;
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E:   // Skip next instruction if a key is pressed
                    printf("SKIP.IF.KEY.PRESSED $0x%X\n",(opcode & 0x0F00));
                    break;
                case 0x0001:   // Skip next instruction if a key is not pressed
                    printf("SKIP.IF.KEY.NOTPRESSED $0x%X\n",(opcode & 0x0F00));
                    break;
            }
        break;
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:   // Move the value of the delay timer into a register
                    printf("GET.DELAY_TIMER $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x000A:   // Wait for a key press and store it in register
                    printf("WAIT.KEY.AND.STORE.IN $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0015:   // Set delay timer to the value in the register
                    printf("SET.DELAY_TIMER $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0018:   // Set sound timer to the value in the register
                    printf("SET.SOUND_TIMER $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x001e:   // Add register value to I (Address register)
                    printf("ADD.TO.I $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0029:   // Set I to the value in the register
                    printf("SET_I.TO $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0033:   // Store the Binary-Coded decimal representation of the register in I, I+1 and I+2
                    printf("BIN-COD-DEC $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0055:   // Store V0-Vx in memory starting at I
                    printf("STORE $0x%X\n",(opcode & 0x0F00));
                break;
                case 0x0065:   // Fill V0-Vx in memory starting at I
                    printf("FILL $0x%X\n", (opcode & 0x0F00));
                break;
            }
        break;
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
            break;
    }


    if(delay_timer > 0)
        --delay_timer;
    if(sound_timer > 0) {
        if (sound_timer == 1)
            printf("BEEP");
        --sound_timer;
    }
}
