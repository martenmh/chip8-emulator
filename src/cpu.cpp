//
// Created by marten on 17-12-19.
//

#include "cpu.h"
#include "chip8.h"
#include <iostream>


static std::vector<unsigned char> binaryCodedDecimal(unsigned short int decimal){
    unsigned char digits[3]{static_cast<unsigned char>(decimal)};
    return {digits[0], digits[1], digits[2]};
}

CPU::CPU(Chip8 *chip8){
    chip8_ = chip8;
    pc      = 0x200;    // Program counter starts at 0x200
    opcode  = 0;        // Reset current opcode
    I       = 0;        // Reset index register
    sp      = 0;        // Reset stack pointer

    for(auto &reg : V)
        reg = 0;

//    for(int i = 0; i < 80; i++)
//        memory[i] = chip8_fontset[i];

    delay_timer = 0;
    sound_timer = 0;
}


void CPU::emulateCycle() {
    /* Store a byte from memory into the 2 byte opcode variable.
       Shift it so it is stored in the high byte.
       Store another byte from memory in to the lower byte */
    opcode =  chip8_->memory[pc] << 8 |  chip8_->memory[pc + 1];

    /*  $0x.... = Value of register
        #0x.... = Value at address
        0x.... = immediate value
        Opcodes are somewhat ordered */
    // Compare the the first nibble (mask the lower 12 bits)
    switch(opcode & 0xF000){
        /* Other */
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0:   // 00E0 Clear screen
                    chip8_->display->clearScreen();
                    pc += 2;
                    break;
                case 0x00EE:   // 00EE Return from subroutine
                    // Set program counter to return address in stack
                    sp--;
                    pc = chip8_->memory.stack[sp];
                    break;
                default:        // 0xxx CALL RCA_1802 at $xxx
                    pc += 2;
                    break;
            }
            break;

        /* Jumps & Calls */
        case 0x1000:   // 1xxx Jump to address $xxx
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:   // 2xxx Call subroutine $xxx
            // Set return address
            chip8_->memory.stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;   // Set opcode to address
            break;
        case 0xB000:   // Bxxx Jump to address #(xxx + V0)
            pc = opcode & 0x0FFF + V0;
            break;

            /* Comparisons */
        case 0x3000:   // 3xnn Skip if Equal (Comparing immediate value to a register, if(Vx == nn))
            if(V[opcode & 0x0F00] == (opcode & 0x00FF))
                pc += 2;
            pc +=2;
            break;
        case 0x4000:   // Skip if Not Equal (Comparing immediate value to a register, if(Vx == nn))
            if(V[opcode & 0x0F00] != (opcode & 0x00FF))
                pc += 2;
            pc +=2;
            break;
        case 0x5000:   // Skip if Equal (Comparing registers, if(Vx == Vy))
            if(V[opcode & 0x0F00] == V[opcode & 0x00F0])
                pc += 2;
            pc +=2;
            break;
        case 0x9000:   // Skip if Not Equal (Comparing registers, if(Vx == Vy))
            if(V[opcode & 0x0F00] != V[opcode & 0x00F0])
                pc += 2;
            pc +=2;
            break;

            /* Immediate values and Register operations */
        case 0x6000:    // 6xnn Assign immediate value to register Vx
            V[opcode & 0x0F00] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:    // 7xnn Add immediate value to Vx, Vx += nn
            V[opcode & 0x0F00] += opcode & 0x00FF;
            pc += 2;
            break;

            /* Register to register operations */
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:   // 8xx0 Move register to register (Vx = Vy)
                    V[opcode & 0x0F00] = V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0001:   // 8xy1 OR x |= y
                    V[opcode & 0x0F00] |= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0002:   // 8xy2 AND x &= y
                    V[opcode & 0x0F00] &= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0003:   // 8xy3 XOR x ^= y
                    V[opcode & 0x0F00] |= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0004:   // 8xy4 Add x += y
                    V[opcode & 0x0F00] += V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0005:   // 8xy5 Subtract x -= y
                    V[opcode & 0x0F00] -= V[opcode & 0x00F0];
                    pc += 2;
                    break;
                case 0x0006:   // 8xy6 Shift right x >>= 1
                    V[opcode & 0x0F00] >>= 1;
                    pc += 2;
                    break;
                case 0x0007:   // 8xy7 Subtract backwards x = y - x
                    V[opcode & 0x0F00] = V[opcode & 0x00F0] - V[opcode & 0x0F00];
                    pc += 2;
                    break;
                case 0x000E:   // 8xyE Shift left x <<= y
                    V[opcode & 0x0F00] <<= 1;
                    pc += 2;
                    break;
            }
            break;

            /* Other */
        case 0xA000:    // Axxx Set i to $xxx
            I = chip8_->memory[opcode & 0x0FFF];
            pc += 2;
            break;
        case 0xC000:    // Cxnn Set Vx to a random number & nn (Vx = rand(0,255) & nn)
            V[opcode & 0x0F00] = (std::rand() % 255) & (opcode & 0x00FF);
            pc += 2;
            break;

            /* Display */
        case 0xD000:   // Dxyn Display value n in (x,y)
            // Set flags register (1 if any pixel that was set has been unset)
            VF = chip8_->display->flipPixel(opcode & 0x0F00, opcode & 0x00F0, opcode & 0x000F);
            sp += 2;
            break;

            /* User input */
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E:   // Ex9E Skip next instruction if the key in Vx is pressed
                    if(chip8_->keyboard.keyIsPressed(opcode & 0x0F00))
                        sp += 2;    // sp += 4 in total
                    sp += 2;
                break;
                case 0x0001:   // ExA1 Skip next instruction if the key in Vx is not pressed
                    if(!chip8_->keyboard.keyIsPressed(opcode & 0x0F00))
                        sp += 2;    // sp += 4 in total
                    sp += 2;
                break;
            }
            break;

        /* Other */
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:   // Fx07 Move the value of the delay timer into register Vx
                    V[opcode & 0x0F00] = delay_timer;
                    pc += 2;
                    break;
                case 0x000A:   // Fx0A Wait for a key press and store it in register Vx
                    V[opcode & 0x0F00] = chip8_->keyboard.waitForKeyPress();
                    pc += 2;
                    break;
                case 0x0015:   // Fx15 Set delay timer to the value in register Vx
                    delay_timer = V[opcode & 0x0F00];
                    pc += 2;
                    break;
                case 0x0018:   // Fx18 Set sound timer to the value in register Vx
                    sound_timer = V[opcode & 0x0F00];
                    pc += 2;
                    break;
                case 0x001e:   // Fx1e Add register value Vx to I (Address register)
                    I += V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x0029:   // Fx29 Set I to the value in register Vx
                    I = V[opcode & 0x0F00];
                    pc += 2;
                break;
                case 0x0033:   // Fx33 Store the Binary-Coded decimal representation of register Vx in I, I+1 and I+2
                    {
                        auto bcdDigits = binaryCodedDecimal(opcode & 0x0F00);
                        chip8_->memory[I] = (short)bcdDigits[0];
                        chip8_->memory[I + 1] = (short)bcdDigits[1];
                        chip8_->memory[I + 2] = (short)bcdDigits[2];
                    }
                    sp += 2;
                break;
                case 0x0055:   // Fx55 Store V0-Vx in memory starting at I
                    for(int i = 0; i <= opcode & 0x0F00; i++)
                        chip8_->memory[I + i] = V[i];

                    pc += 2;
                    break;
                case 0x0065:   // Fx65 Fill V0-Vx in memory starting at I
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

void CPU::getOpcode(unsigned short opcode, std::ostream &os){
    switch(opcode & 0xF000){
        /* Other */
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0: os << "CLS"; break;
                case 0x00EE: os << "RET"; break;
                default: os << "CALL.RCA_1802 " << (opcode & 0x0FFF); break;
            }
            break;

            /* Jumps & Calls */
        case 0x1000: os << "JMP #0x" << (opcode & 0x0FFF); break;
        case 0x2000: os << "CALL #0x" << (opcode & 0x0FFF); break;
        case 0xB000: os << "JMP #0x" <<  (opcode & 0x0FFF) << "(V0)"; break;

            /* Comparisons */
        case 0x3000: os << "SKIP.EQ $0x" << (opcode & 0x0F00) << ", 0x" << (opcode & 0x00FF); break;
        case 0x4000: os << "SKIP.NE $0x" << (opcode & 0x0F00) << ", 0x" << (opcode & 0x00FF); break;
        case 0x5000: os << "SKIP.EQ $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00F0); break;
        case 0x9000: os << "SKIP.NE $0x%X" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00F0); break;

            /* Immediate values and Register operations */
        case 0x6000: os << "MOV $0x" << (opcode & 0x0F00) << ", 0x" << (opcode & 0x0F00); break;
        case 0x7000: os << "ADD $0x" << (opcode & 0x0F00) << ", #0x" << (opcode & 0x0F00); break;

            /* Register to register operations */
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000: os << "MOV $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0001: os << "OR $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0002: os << "AND $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0003: os << "XOR $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0004: os << "ADD $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0005: os << "SUB $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0006: os << "SHR $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00FF); break;
                case 0x0007: os << "SUBB $0x" << (opcode & 0x0F00); break;
                case 0x000E: os << "SHL $0x" << (opcode & 0x0F00) << ", $0x" <<  (opcode & 0x00FF); break;
            }
            break;

            /* Other */
        case 0xA000: os << "SETI #0x" << (opcode & 0x0FFF); break;
        case 0xC000: os << "RAND $0x" << (opcode & 0x0F00) << ", 0x" << (opcode & 0x00FF); break;

            /* Display */
        case 0xD000: os << "DISP $0x" << (opcode & 0x0F00) << ", $0x" << (opcode & 0x00F0) << ", #0x" << (opcode & 0x000F); break;

            /* User input */
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E: os << "SKIP.IF.KEY.PRESSED $0x" << (opcode & 0x0F00); break;
                case 0x0001: os << "SKIP.IF.KEY.NOTPRESSED $0x" << (opcode & 0x0F00); break;
            }
            break;

            /* Other */
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007: os << "GET.DELAY_TIMER $0" << (opcode & 0x0F00); break;
                case 0x000A: os << "WAIT.KEY.AND.STORE.IN $0" << (opcode & 0x0F00); break;
                case 0x0015: os << "SET.DELAY_TIMER $0" << (opcode & 0x0F00); break;
                case 0x0018: os << "SET.SOUND_TIMER $0" << (opcode & 0x0F00); break;
                case 0x001e: os << "ADD.TO.I $0" << (opcode & 0x0F00); break;
                case 0x0029: os << "SET_I.TO $0" << (opcode & 0x0F00); break;
                case 0x0033: os << "BIN-COD-DEC $0" << (opcode & 0x0F00); break;
                case 0x0055: os << "STORE $0" << (opcode & 0x0F00); break;
                case 0x0065: os << "FILL $0x" <<  (opcode & 0x0F00); break;
            }
            break;
        default: os << "Unknown opcode: 0x" << opcode;
    }

    os << "\n";
}

void CPU::getDisassembly(std::ostream &os) {
    auto swapPC = pc;

    while(chip8_->memory[swapPC] != '\0'){
        opcode =  chip8_->memory[swapPC] << 8 |  chip8_->memory[swapPC + 1];
        // Compare the the first nibble (mask the lower 12 bits)
        getOpcode(opcode, os);
        swapPC += 2;
    }
}

#include <iomanip>  // Formatting
using namespace std;

void CPU::getCPUInfo(std::ostream &os) {
    const int width = 23;
    os << setw(width) << left << "Current Opcode" << opcode << endl;
    // Registers (8 bit)
    os << setw(width) << left << "V0" << (int)V0 << endl;
    os << setw(width) << left << "V1" << (int)V1 << endl;
    os << setw(width) << left << "V2" << (int)V2 << endl;
    os << setw(width) << left << "V3" << (int)V3 << endl;
    os << setw(width) << left << "V4" << (int)V4 << endl;
    os << setw(width) << left << "V5" << (int)V5 << endl;
    os << setw(width) << left << "V6" << (int)V6 << endl;
    os << setw(width) << left << "V7" << (int)V7 << endl;
    os << setw(width) << left << "V8" << (int)V8 << endl;
    os << setw(width) << left << "V9" << (int)V9 << endl;
    os << setw(width) << left << "VA" << (int)VA << endl;
    os << setw(width) << left << "VB" << (int)VB << endl;
    os << setw(width) << left << "VC" << (int)VC << endl;
    os << setw(width) << left << "VD" << (int)VD << endl;
    os << setw(width) << left << "VE" << (int)VE << endl;
    os << setw(width) << left << "VF (Flags register)" << (int)VF << endl;
    // Register (16 bit)
    os << setw(width) << left << "I  (Address register)" << (int)I << endl;
    os << setw(width) << left << "pc (Program counter)" << (int)pc << endl;
    // Timers
    os << setw(width) << left << "delay_timer" << (int)delay_timer << endl;
    os << setw(width) << left << "sound_timer" << (int)sound_timer << endl;

    os << setw(width) << left << "Stack pointer" << (int)sp << endl;
}

CPU::CPU() {

}




