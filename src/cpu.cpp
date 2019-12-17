//
// Created by marten on 17-12-19.
//

#include "chip8_cpu.h"

void initialize();
void emulateCycle();

void Chip8_cpu::initialize() {
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
