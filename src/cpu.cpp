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

#define DEFAULT_UNKNOWN_OP default: chip8_->quit(UNKNOWN_OPCODE, "Unknown opcode: 0x" + Chip8::hexToReadableOpcode(opcode)); break;
//#define DEFAULT_UNKNOWN_OP default: sp += 2; break;
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
        DEFAULT_UNKNOWN_OP
        /* Other */
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0:   // 00E0 Clear screen
                    chip8_->display->clearScreen();
                    pc += 2;
                    break;
                case 0x00EE:    // 00EE Return from subroutine
                    // Set program counter to return address at the top of the stack
                    sp--;
                    pc = chip8_->stack[sp];
                    pc += 2;
                    break;
                default:        // 0xxx syscall RCA_1802 at $xxx
                    // If xxx = 0, it's likely empty memory so just quit the emulator
                    if((opcode & 0x0FFF) == 0) {
                        chip8_->quit(1, "Unknown opcode: 0x" + Chip8::hexToReadableOpcode(opcode));
                        break;
                    }
                    // If is not 0 Ignore as we're not using 0x0xxx
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
            chip8_->stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;   // Set opcode to address
            break;

            /* Comparisons */
        case 0x3000:   // 3xnn Skip if Equal (Comparing immediate value to a register, if(Vx == nn))
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x4000:   // Skip if Not Equal (Comparing immediate value to a register, if(Vx != nn))
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        case 0x5000:   // Skip if Equal (Comparing registers, if(Vx == Vy))
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
            break;

        /* Immediate values and Register operations */
        case 0x6000:    // 6xnn Assign immediate value to register Vx
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:    // 7xnn Add immediate value to Vx, Vx += nn
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            break;

        /* Register to register operations */
        case 0x8000:
            switch(opcode & 0x000F){
                DEFAULT_UNKNOWN_OP
                case 0x0000:   // 8xx0 Move register to register (Vx = Vy)
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0001:   // 8xy1 OR x |= y
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0002:   // 8xy2 AND x &= y
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0003:   // 8xy3 XOR x ^= y
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0004:   // 8xy4 Add x += y
                    // Check how much the char can still hold by (255 - VX), if the remaining number to be added is larger than the value it can hold there is a carry
                    // Vy > (255 - Vx) == carry
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
                        VF = 1; // Carry
                    else
                        VF = 0;

                    pc += 2;
                break;
                case 0x0005:   // 8xy5 Subtract x -= y
                    // If y is larger than x set borrow flag
                    if(V[(opcode & 0x00F0) >> 4] > (V[(opcode & 0x0F00) >> 8]))
                        VF = 1; // Borrow
                    else
                        VF = 0;

                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];

                    pc += 2;
                    break;
                case 0x0006:   // 8xy6 Shift right x >>= 1  (stores least significant bit in VF)
                    VF = ((opcode & 0x0F00) >> 8) & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                    break;
                case 0x0007:   // 8xy7 Subtract backwards x = y - x
                    // If y is larger than x set borrow flag
                    if((V[(opcode & 0x0F00) >> 8]) > V[(opcode & 0x00F0) >> 4])
                        VF = 0; // Borrow
                    else
                        VF = 1;

                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x000E:   // 8xyE Shift left x <<= y   (stores most significant bit in VF)
                    VF = ((opcode & 0x0F00) >> 8) >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                    break;

            }
            break;

        /* Comparison */
        case 0x9000:   // Skip if Not Equal (Comparing registers, if(Vx != Vy))
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
                pc += 4;
            else
                pc += 2;
        break;


            /* Other */
        case 0xA000:    // Axxx Set i to $xxx
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:   // Bxxx Jump to address #(xxx + V0)
            pc = (opcode & 0x0FFF) + V0;
            break;
        case 0xC000:    // Cxnn Set Vx to a random number & nn (Vx = rand(0,255) & nn)
            V[(opcode & 0x0F00) >> 8] = (std::rand() % (0xFF + 1)) & (opcode & 0x00FF);
            pc += 2;
            break;

        /* Display */
        case 0xD000:   // Dxyn Display value n in (x,y)
            // Set flags register (1 if any pixel that was set has been unset)
            VF = chip8_->display->display(V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], opcode & 0x000F);
            pc += 2;
            break;

            /* User input */
        case 0xE000:
            switch(opcode & 0x000F){
                DEFAULT_UNKNOWN_OP
                case 0x000E:   // Ex9E Skip next instruction if the key in Vx is pressed
                    if(chip8_->keyboard.keyIsPressed((opcode & 0x0F00) >> 8) != 0)
                        pc += 4;    // pc += 4 in total
                    else
                        pc += 2;
                break;
                case 0x0001:   // ExA1 Skip next instruction if the key in Vx is not pressed
                    if(chip8_->keyboard.keyIsPressed((opcode & 0x0F00) >> 8) == 1)
                        pc += 4;    // pc += 4 in total
                    else
                        pc += 2;
                break;
            }
            break;

        /* Other */
        case 0xF000:
            switch(opcode & 0x00FF){
                DEFAULT_UNKNOWN_OP
                case 0x0007:   // Fx07 Move the value of the delay timer into register Vx
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    break;
                case 0x000A:   // Fx0A Wait for a key press and store it in register Vx
                    V[(opcode & 0x0F00) >> 8] = chip8_->keyboard.waitForKeyPress();
                    pc += 2;
                    break;
                case 0x0015:   // Fx15 Set delay timer to the value in register Vx
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x0018:   // Fx18 Set sound timer to the value in register Vx
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                    break;
                case 0x001E:   // Fx1e Add register value Vx to I (Address register)
                if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
                    VF = 1;
                else
                    VF = 0;
                    I += V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;
                case 0x0029:   // Fx29 Sets I to the address of the sprite for the character in VX
                    I = chip8_->getAddressOfChar(V[(opcode & 0x0F00) >> 8]);
                    pc += 2;
                break;
                case 0x0033:   // Fx33 Store the Binary-Coded decimal representation of register Vx in I, I+1 and I+2
                    {
                        auto bcdDigits = binaryCodedDecimal(opcode & 0x0F00);
                        chip8_->memory[I] = (short)bcdDigits[0];
                        chip8_->memory[I + 1] = (short)bcdDigits[1];
                        chip8_->memory[I + 2] = (short)bcdDigits[2];
                    }
                    pc += 2;
                break;
                case 0x0055:   // Fx55 Store V0-Vx in memory starting at I
                    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
                        chip8_->memory[I + i] = V[i];
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;
                case 0x0065:   // Fx65 Fill V0-Vx from memory starting at I
                    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++)
                        V[i] = chip8_->memory[I + i];
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc += 2;
                    break;
            }
            break;
    }

    if(delay_timer > 0)
        --delay_timer;
    if(sound_timer > 0) {
        if (sound_timer == 1)
            std::cout << "\033[1;31mBEEP\033[0m\n" << std::endl;

        --sound_timer;
    }
}
#define SECOND_NIBBLE ((opcode & 0x0F00) >> 8)
#define THIRD_NIBBLE ((opcode & 0x00F0) >> 4)
#undef DEFAULT_UNKNOWN_OP
#define DEFAULT_UNKNOWN_OP default: os << "Unknown opcode: 0x" << Chip8::hexToReadableOpcode(opcode); break;

void CPU::getOpcode(unsigned short opcode, std::ostream &os){
    switch(opcode & 0xF000){
        DEFAULT_UNKNOWN_OP
        /* Other */
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0: os << "CLS"; break;    // 00E0
                case 0x00EE: os << "RET"; break;    // 00EE
                default: os << "CALL.RCA_1802 " << (opcode & 0x0FFF); break;    // 0NNN
            }
            break;

        /* Jumps & Calls */
        case 0x1000: os << "JMP " << (opcode & 0x0FFF); break;                                      // 1NNN
        case 0x2000: os << "CALL " << (opcode & 0x0FFF); break;                                     // 2NNN
        case 0xB000: os << "JMP " <<  (opcode & 0x0FFF) << "(V0)"; break;                           // BNNN (JMP NNN + V0)

        /* Comparisons */
        case 0x3000: os << "SKIP.EQ V" << SECOND_NIBBLE << ", " << (opcode & 0x00FF); break;       // 3XNN
        case 0x4000: os << "SKIP.NE V" << SECOND_NIBBLE << ", " << (opcode & 0x00FF); break;       // 4XNN
        case 0x5000: os << "SKIP.EQ V" << SECOND_NIBBLE << ", V" << THIRD_NIBBLE; break;           // 5XY0
        case 0x9000: os << "SKIP.NE V" << SECOND_NIBBLE << ", V" << THIRD_NIBBLE; break;           // 9XY0

        /* Immediate values and Register operations */
        case 0x6000: os << "MOV V" << SECOND_NIBBLE << ", " << (opcode & 0x00FF); break;            // 6XNN
        case 0x7000: os << "ADD V" << SECOND_NIBBLE << ", " << (opcode & 0x00FF); break;            // 7XNN

        /* Register to register operations */
        case 0x8000:
            switch(opcode & 0x000F){
                DEFAULT_UNKNOWN_OP
                case 0x0000: os << "MOV V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY0
                case 0x0001: os << "OR V"  << SECOND_NIBBLE << ", " <<  THIRD_NIBBLE; break;        // 8XY1
                case 0x0002: os << "AND V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY2
                case 0x0003: os << "XOR V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY3
                case 0x0004: os << "ADD V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY4
                case 0x0005: os << "SUB V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY5
                case 0x0006: os << "SHR V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;         // 8XY6
                case 0x0007: os << "SUBB V" << SECOND_NIBBLE << ", " << THIRD_NIBBLE; break;        // 8XY7
                case 0x000E: os << "SHL V" << SECOND_NIBBLE << ", " <<  (opcode & 0x00FF); break;   // 8XYE
            }
            break;

        /* Other */
        case 0xA000: os << "SETI " << (opcode & 0x0FFF); break;                                     // ANNN
        case 0xC000: os << "RAND " << SECOND_NIBBLE << ", " << (opcode & 0x00FF); break;            // CXNN

            /* Display */
        case 0xD000: os << "DISP V" << SECOND_NIBBLE << ", V" << THIRD_NIBBLE << ", " << (opcode & 0x000F); break;  // DXYN

            /* User input */
        case 0xE000:
            switch(opcode & 0x000F){
                DEFAULT_UNKNOWN_OP
                case 0x000E: os << "SKIP.IF.KEY.PRESSED $0x" << SECOND_NIBBLE; break;           // EX9E
                case 0x0001: os << "SKIP.IF.KEY.NOTPRESSED $0x" << SECOND_NIBBLE; break;        // EXA1
            }
            break;

            /* Other */
        case 0xF000:
            switch(opcode & 0x00FF){
                DEFAULT_UNKNOWN_OP
                case 0x0007: os << "GET.DELAY_TIMER $0" << SECOND_NIBBLE; break;                // FX07
                case 0x000A: os << "WAIT.KEY.AND.STORE.IN $0" << SECOND_NIBBLE; break;          // FX0A
                case 0x0015: os << "SET.DELAY_TIMER $0" << SECOND_NIBBLE; break;                // FX15
                case 0x0018: os << "SET.SOUND_TIMER $0" << SECOND_NIBBLE; break;                // FX18
                case 0x001e: os << "ADD.TO.I $0" << SECOND_NIBBLE; break;                       // FX1E
                case 0x0029: os << "SET_I.FONTADDR " << SECOND_NIBBLE; break;                       // FX29
                case 0x0033: os << "BIN-COD-DEC $0" << SECOND_NIBBLE; break;                    // FX33
                case 0x0055: os << "STORE $0" << SECOND_NIBBLE; break;                          // FX55
                case 0x0065: os << "FILL $0x" <<  SECOND_NIBBLE; break;                         // FX99
            }
            break;
    }

    os << std::endl;
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

void CPU::getCPUInfo(std::ostream &os, unsigned int width) {
    os << setw(width) << left << "Current Opcode" << Chip8::hexToReadableOpcode(opcode) << endl;
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
    os << setw(width) << left << "pc (Program counter)" << (int)pc <<  endl;
    // Timers
    os << setw(width) << left << "delay_timer" << (int)delay_timer << endl;
    os << setw(width) << left << "sound_timer" << (int)sound_timer << endl;

    os << setw(width) << left << "Stack pointer" << (int)sp << endl;
}

CPU::CPU() {

}
