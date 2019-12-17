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

void CPU::emulateCycle() {

    /* Store a byte from memory into the 2 byte opcode variable.
       Shift it so it is stored in the high byte.
       Store another byte from memory in to the lower byte */
    opcode = chip8_->memory[pc] << 8 | chip8_->memory[pc+1];

    switch(opcode & 0xF000){

    }

}
