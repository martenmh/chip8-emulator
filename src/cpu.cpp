//
// Created by marten on 17-12-19.
//

#include "cpu.h"
#include "chip8.h"

void CPU::initialize() {
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
typedef unsigned char nibble;

unsigned short createByteFromNibbles(nibble one, nibble two){
    return one | two;
}
unsigned short createThreeBytesFromNibbles(nibble one, nibble two, nibble three){
    return one | two | three;
}

#include <iostream>
void CPU::emulateCycle() {
    /* Store a byte from memory into the 2 byte opcode variable.
       Shift it so it is stored in the high byte.
       Store another byte from memory in to the lower byte */
    opcode = chip8_->memory[pc] << 8 | chip8_->memory[pc+1];

    // Get the 4 nibbles of the 2 bytes, each representing a hexadecimal character
    nibble firstNibble = chip8_->memory[pc] >> 4;
    nibble secondNibble = chip8_->memory[pc] & 0x0F;
    nibble thirdNibble = chip8_->memory[pc+1] >> 4;
    nibble fourthNibble = chip8_->memory[pc+1] & 0x0F;

    // $ = Value of register
    // # = Immediate value
    // Compare the first character
    switch(firstNibble){
        case 0x0:
            // Compare the last character
            switch(fourthNibble){
                case 0x0:   // Clear screen
                    std::cout << "CLS" << std::endl;
                break;
                case 0xE:   // Return from subroutine
                    std::cout << "RET" << std::endl;
                break;
            }
        break;
        case 0x1:   // Jump to address
            std::cout << "JMP $" << secondNibble << thirdNibble << fourthNibble << std::endl;
        break;
        case 0x2:   // Call subroutine
            std::cout << "CALL $" << secondNibble << thirdNibble << fourthNibble << std::endl;
        break;
        case 0xB:   // Jump to address + V0
            std::cout << "JMP $" << secondNibble << thirdNibble << fourthNibble << "(V0)" << std::endl;
        break;
        case 0x3:   // Skip if Equal
            std::cout << "SKIP.EQ $" << secondNibble << ", #" << thirdNibble << fourthNibble << std::endl;
        break;
        case 0x4:   // Skip if Not Equal
            std::cout << "SKIP.NE $" << secondNibble << ", #" << thirdNibble << std::endl;
        break;
        case 0x5:   // Skip if Equal (Comparing registers)
            std::cout << "SKIP.EQ $" << secondNibble << ", $" << thirdNibble << std::endl;
        break;
        case 0x9:   // Skip if Not Equal (Comparing registers)
            std::cout << "SKIP.NE $" << secondNibble << ", $" << thirdNibble << std::endl;
        break;
        case 0x8:
            switch(fourthNibble){
                case 0x0:   // Move register to register
                    std::cout << "MOV $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x1:   // OR
                    std::cout << "OR $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x2:   // AND
                    std::cout << "AND $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x3:   // XOR
                    std::cout << "XOR $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x4:   // Add
                    std::cout << "ADD $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x5:   // Subtract
                    std::cout << "SUB $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x6:   // Shift right
                    std::cout << "SHR $" << secondNibble << ", $" << thirdNibble << std::endl;
                    break;
                case 0x7:   // Subtract backwards
                    std::cout << "SUBB $" << secondNibble << std::endl;
                    break;
                case 0xE:   // Shift left
                    std::cout << "SHL $" << secondNibble << std::endl;
                    break;
            }
        break;
        case 0xD:   // Display value in (X,Y)
            std::cout << "DISP $" << secondNibble << ", $" << thirdNibble << ", #" << fourthNibble << std::endl;
        break;
        case 0xE:
            switch(fourthNibble){
                case 0xE:   // Skip next instruction if a key is pressed
                    std::cout << "SKIP.IF.KEY.PRESSED $" << secondNibble << std::endl;
                    break;
                case 0x1:   // Skip next instruction if a key is not pressed
                    std::cout << "SKIP.IF.KEY.NOTPRESSED $" << secondNibble << std::endl;
                    break;
            }
        break;
        case 0xF:
            switch(thirdNibble | fourthNibble){
                case 0x07:   // Move the value of the delay timer into a register
                    std::cout << "GET.DELAY_TIMER $" << secondNibble << std::endl;
                break;
                case 0x0A:   // Wait for a key press and store it in register
                    std::cout << "WAIT.KEY.AND.STORE.IN $" << secondNibble << std::endl;
                break;
                case 0x15:   // Set delay timer to the value in the register
                    std::cout << "SET.DELAY_TIMER $" << secondNibble << std::endl;
                break;
                case 0x18:   // Set sound timer to the value in the register
                    std::cout << "SET.SOUND_TIMER $" << secondNibble << std::endl;
                break;
                case 0x1e:   // Add register value to I (Address register)
                    std::cout << "ADD.TO.I $" << secondNibble << std::endl;
                break;
                case 0x29:   // Set I to the value in the register
                    std::cout << "SET_I.TO $" << secondNibble << std::endl;
                break;
                case 0x33:   // Store the Binary-Coded decimal representation of the register in I, I+1 and I+2
                    std::cout << "BIN-COD-DEC $" << secondNibble << std::endl;
                break;
                case 0x55:   // Store V0-Vx in memory starting at I
                    std::cout << "STORE $" << secondNibble << std::endl;
                break;
                case 0x65:   // Fill V0-Vx in memory starting at I
                    std::cout << "FILL $" << secondNibble << std::endl;
                break;
            }
        break;
        default:
            std::cerr << "Unknown opcode: " << opcode << std::endl;
        break;
    }
    pc += 2;

    if(delay_timer > 0)
        --delay_timer;
    if(sound_timer > 0) {
        if (sound_timer == 1)
            std::cout << "BEEP" << std::endl;
        --sound_timer;
    }
}
