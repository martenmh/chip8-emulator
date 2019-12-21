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
    std::cout << (opcode & 0xF000) << (opcode & 0x0F00) << (opcode & 0x00F0) << (opcode & 0x000F) << std::endl;
    // $ = Value of register
    // # = Immediate value
    // Compare the first character
    switch(opcode & 0xF000){
        case 0xA000:
            std::cout << "SETI " << (opcode & 0x0FFF) << std::endl;
        break;
        case 0xC000:
            std::cout << "RAND $" << (opcode & 0x0F00) << ", #" << (opcode & 0x00FF) << std::endl;
        break;
        case 0x0000:
            // Compare the last 2 characters
            switch(opcode & 0x00FF){
                case 0x00E0:   // 00E0 Clear screen
                    std::cout << "CLS" << std::endl;
                break;
                case 0x00EE:   // 00EE Return from subroutine
                    std::cout << "RET" << std::endl;
                break;
                default:        // 0xxx
                    std::cout << "CALL.RCA_1802 " << (opcode & 0x0FFF) << std::endl;
                break;
            }
        break;
        case 0x1000:   // Jump to address
            std::cout << "JMP $" << (opcode & 0x0FFF) << std::endl;
        break;
        case 0x2000:   // Call subroutine
            std::cout << "CALL $" << (opcode & 0x0FFF) << std::endl;
        break;
        case 0xB000:   // Jump to address + V0
            std::cout << "JMP $" << (opcode & 0x0FFF) << "(V0)" << std::endl;
        break;
        case 0x3000:   // Skip if Equal (Comparing immediate value to a register)
            std::cout << "SKIP.EQ $" << (opcode & 0x0F00) << ", #" << (opcode & 0x00FF) << std::endl;
        break;
        case 0x4000:   // Skip if Not Equal (Comparing immediate value to a register)
            std::cout << "SKIP.NE $" << (opcode & 0x0F00) << ", #" << (opcode & 0x00FF) << std::endl;
        break;
        case 0x5000:   // Skip if Equal (Comparing registers)
            std::cout << "SKIP.EQ $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00F0) << std::endl;
        break;
        case 0x9000:   // Skip if Not Equal (Comparing registers)
            std::cout << "SKIP.NE $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00F0) << std::endl;
        break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000:   // 8xx0 Move register to register
                    std::cout << "MOV $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0001:   // 8xy1 OR
                    std::cout << "OR $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0002:   // 8xy2 AND
                    std::cout << "AND $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0003:   // 8xy3 XOR
                    std::cout << "XOR $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0004:   // 8xy4 Add
                    std::cout << "ADD $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0005:   // 8xy5 Subtract
                    std::cout << "SUB $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    break;
                case 0x0006:   // 8xy6 Shift right
                    std::cout << "SHR $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    //std::cout << "SHL $" << (opcode & 0x0F00) << std::endl;
                    break;
                case 0x0007:   // 8xy7 Subtract backwards
                    std::cout << "SUBB $" << (opcode & 0x0F00) << std::endl;
                    break;
                case 0x000E:   // 8xyE Shift left
                    std::cout << "SHL $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00FF) << std::endl;
                    //std::cout << "SHL $" << (opcode & 0x0F00) << std::endl;
                    break;
            }
        break;
        case 0xD000:   // Dxyn Display value n in (x,y)
            std::cout << "DISP $" << (opcode & 0x0F00) << ", $" << (opcode & 0x00F0) << ", #" << (opcode & 0x000F) << std::endl;
        break;
        case 0xE000:
            switch(opcode & 0x000F){
                case 0x000E:   // Skip next instruction if a key is pressed
                    std::cout << "SKIP.IF.KEY.PRESSED $" << (opcode & 0x0F00) << std::endl;
                    break;
                case 0x0001:   // Skip next instruction if a key is not pressed
                    std::cout << "SKIP.IF.KEY.NOTPRESSED $" << (opcode & 0x0F00) << std::endl;
                    break;
            }
        break;
        case 0xF000:
            switch(opcode & 0x00FF){
                case 0x0007:   // Move the value of the delay timer into a register
                    std::cout << "GET.DELAY_TIMER $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x000A:   // Wait for a key press and store it in register
                    std::cout << "WAIT.KEY.AND.STORE.IN $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0015:   // Set delay timer to the value in the register
                    std::cout << "SET.DELAY_TIMER $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0018:   // Set sound timer to the value in the register
                    std::cout << "SET.SOUND_TIMER $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x001e:   // Add register value to I (Address register)
                    std::cout << "ADD.TO.I $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0029:   // Set I to the value in the register
                    std::cout << "SET_I.TO $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0033:   // Store the Binary-Coded decimal representation of the register in I, I+1 and I+2
                    std::cout << "BIN-COD-DEC $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0055:   // Store V0-Vx in memory starting at I
                    std::cout << "STORE $" << (opcode & 0x0F00) << std::endl;
                break;
                case 0x0065:   // Fill V0-Vx in memory starting at I
                    std::cout << "FILL $" << (opcode & 0x0F00) << std::endl;
                break;
            }
        break;
        default:
            std::cerr << "Unknown opcode: " << opcode << std::endl;
        break;
    }


    if(delay_timer > 0)
        --delay_timer;
    if(sound_timer > 0) {
        if (sound_timer == 1)
            std::cout << "BEEP" << std::endl;
        --sound_timer;
    }
}
