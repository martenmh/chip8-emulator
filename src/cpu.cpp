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

    /* $ = Value of register
       # = Value at address
       0x... = immediate value
       Opcodes are somewhat ordered
    */
    // Compare the the first nibble (mask the lower 12 bits)
    switch(opcode & 0xF000){
        /* Other */
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0:   // 00E0 Clear screen
                    printf("CLS");
                    chip8_->display.clearScreen();
                    sp += 2;
                    break;
                case 0x00EE:   // 00EE Return from subroutine
                    printf("RET");
                    sp += 2;
                    break;
                default:        // 0xxx CALL RCA_1802 at $xxx
                    printf("CALL.RCA_1802 ",(opcode & 0x0FFF));
                    sp += 2;
                    break;
            }
        break;

        /* Jumps & Calls */
        case 0x1000:   // 1xxx Jump to address $xxx
            printf("JMP #0x%X\n",(opcode & 0x0FFF));
            sp += 2;
        break;
        case 0x2000:   // 2xxx Call subroutine $xxx
            printf("CALL #0x%X\n",(opcode & 0x0FFF));
            sp += 2;
        break;
        case 0xB000:   // Bxxx Jump to address #(xxx + V0)
            printf("JMP #0x%X(V0)\n",(opcode & 0x0FFF));
            sp += 2;
        break;

        /* Comparisons */
        case 0x3000:   // 3xnn Skip if Equal (Comparing immediate value to a register, if(Vx == nn))
            printf("SKIP.EQ $0x%X, 0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
            if(V[opcode & 0x0F00] == opcode & 0x00FF) {
                pc += 4;
                break;
            }
            pc +=2;
        break;
        case 0x4000:   // Skip if Not Equal (Comparing immediate value to a register, if(Vx == nn))
            printf("SKIP.NE $0x%X, 0x%X\n",(opcode & 0x0F00), (opcode & 0x00FF));
            if(V[opcode & 0x0F00] != opcode & 0x00FF) {
                pc += 4;
                break;
            }
            pc +=2;
        break;
        case 0x5000:   // Skip if Equal (Comparing registers, if(Vx == Vy))
            printf("SKIP.EQ $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0));
            if(V[opcode & 0x0F00] == V[opcode & 0x00F0]) {
                pc += 4;
                break;
            }
            pc +=2;
        break;
        case 0x9000:   // Skip if Not Equal (Comparing registers, if(Vx == Vy))
            printf("SKIP.NE $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0));
            if(V[opcode & 0x0F00] != V[opcode & 0x00F0]) {
                pc += 4;
                break;
            }
            pc +=2;
        break;

        /* Immediate values and Register operations */
        case 0x6000:    // 6xnn Assign immediate value to register Vx
            printf("MOV $0x%X, 0x%X\n", (opcode & 0x0F00), (opcode & 0x0F00));
            V[opcode & 0x0F00] = opcode & 0x00FF;
            pc += 2;
        break;
        case 0x7000:    // 7xnn Add immediate value to Vx, Vx += nn
            printf("ADD $0x%X, #0x%X\n", (opcode & 0x0F00), (opcode & 0x0F00));
            V[opcode & 0x0F00] += opcode & 0x00FF;
            pc += 2;
        break;

        /* Register to register operations */
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:   // 8xx0 Move register to register (Vx = Vy)
                    printf("MOV $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] = V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0001:   // 8xy1 OR x |= y
                    printf("OR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] |= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0002:   // 8xy2 AND x &= y
                    printf("AND $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] &= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0003:   // 8xy3 XOR x ^= y
                    printf("XOR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] |= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0004:   // 8xy4 Add x += y
                    printf("ADD $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] += V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0005:   // 8xy5 Subtract x -= y
                    printf("SUB $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] -= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0006:   // 8xy6 Shift right x >>= 1
                    printf("SHR $0x%X, $0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
                    V[opcode & 0x0F00] >>= 1;
                    pc += 2;
                    break;
                case 0x0007:   // 8xy7 Subtract backwards x = y - x
                    printf("SUBB $0x%X\n",(opcode & 0x0F00));
                    V[opcode & 0x0F00] = V[opcode & 0x00F0] - V[opcode & 0x0F00];
                    pc += 2;
                    break;
                case 0x000E:   // 8xyE Shift left x <<= y
                    printf("SHL $0x%X, $0x%X\n",(opcode & 0x0F00), (opcode & 0x00FF));
                    V[opcode & 0x0F00] <<= 1;
                    pc += 2;
                    break;
            }
        break;

        /* Other */
        case 0xA000:    // Axxx Set i to $xxx
            printf("SETI #0x%X",(opcode & 0x0FFF));
            I = chip8_->memory[opcode & 0x0FFF];
            pc += 2;
            break;
        case 0xC000:    // Cxnn Set Vx to a random number & nn (Vx = rand(0,255) & nn)
            printf("RAND $0x%X, 0x%X\n",(opcode & 0x0F00),(opcode & 0x00FF));
            V[opcode & 0x0F00] = (std::rand() % 255) & (opcode & 0x00FF);
            pc += 2;
            break;

        /* Display */
        case 0xD000:   // Dxyn Display value n in (x,y)
            printf("DISP $0x%X, $0x%X, #0x%X\n",(opcode & 0x0F00),(opcode & 0x00F0),(opcode & 0x000F));
            sp += 2;
        break;

        /* User input */
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E:   // Ex9E Skip next instruction if the key in Vx is pressed
                    printf("SKIP.IF.KEY.PRESSED $0x%X\n",(opcode & 0x0F00));
                    sp += 2;
                break;
                case 0x0001:   // ExA1 Skip next instruction if the key in Vx is not pressed
                    printf("SKIP.IF.KEY.NOTPRESSED $0x%X\n",(opcode & 0x0F00));
                    sp += 2;
                break;
            }
        break;

        /* Other */
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:   // Fx07 Move the value of the delay timer into register Vx
                    printf("GET.DELAY_TIMER $0x%X\n",(opcode & 0x0F00));
                    V[opcode & 0x0F00] = delay_timer;
                    pc += 2;
                break;
                case 0x000A:   // Fx0A Wait for a key press and store it in register Vx
                    printf("WAIT.KEY.AND.STORE.IN $0x%X\n",(opcode & 0x0F00));
                    // TODO:
                    V[opcode & 0x0F00] = 3;
                    pc += 2;
                break;
                case 0x0015:   // Fx15 Set delay timer to the value in register Vx
                    printf("SET.DELAY_TIMER $0x%X\n",(opcode & 0x0F00));
                    delay_timer = V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x0018:   // Fx18 Set sound timer to the value in register Vx
                    printf("SET.SOUND_TIMER $0x%X\n",(opcode & 0x0F00));
                    sound_timer = V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x001e:   // Fx1e Add register value Vx to I (Address register)
                    printf("ADD.TO.I $0x%X\n",(opcode & 0x0F00));
                    I += V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x0029:   // Fx29 Set I to the value in register Vx
                    printf("SET_I.TO $0x%X\n",(opcode & 0x0F00));
                    I = V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x0033:   // Fx33 Store the Binary-Coded decimal representation of register Vx in I, I+1 and I+2
                    printf("BIN-COD-DEC $0x%X\n",(opcode & 0x0F00));
                    // TODO:
                    //bcd(V[opcode & 0x0F00]);
                    sp += 2;
                break;
                case 0x0055:   // Fx55 Store V0-Vx in memory starting at I
                    printf("STORE $0x%X\n",(opcode & 0x0F00));
                    for(int i = 0; i <= opcode & 0x0F00; i++)
                        chip8_->memory[I + i] = V[i];

                    pc += 2;
                break;
                case 0x0065:   // Fx65 Fill V0-Vx in memory starting at I
                    printf("FILL $0x%X\n", (opcode & 0x0F00));
                    for(int i = 0; i <= opcode & 0x0F00; i++)
                        chip8_->memory[I + i] = V[i];

                    pc += 2;
                break;
            }
        break;
        default:
            // Unknown
            printf ("Unknown opcode: 0x%X\n", opcode);
            sp += 2;
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


