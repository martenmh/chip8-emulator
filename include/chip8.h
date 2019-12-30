//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_CHIP8_EMULATOR_H
#define CHIP_8_EMULATOR_CHIP8_EMULATOR_H

#include <memory>
#include <SDL2/SDL.h>

#include "cpu.h"
#include "memory.h"
#include "keyboard.h"
#include "display.h"
#include "keyboard.h"

class Chip8 {
public:
    Chip8();
    ~Chip8();

    /**
       Load a program file into memory
       @param name The path name of the game
     */
    void loadProgram(std::string pathName);
    inline bool isRunning(){ return enabled; }

    // The various Chip8 parts

    // Memory
    // 4kb of bytes (= unsigned char) 0x000-0x1000
    unsigned char memory[4096];
    // Stack to store return addresses when subroutines are called (call stack)
    unsigned short stack[16];   // Call stack

    CPU cpu;
    Display *display;   // Output
    Keyboard keyboard;  // Input

    inline void quit(){ enabled = false; }

private:
    bool enabled;
    bool paused;
};


#endif //CHIP_8_EMULATOR_CHIP8_EMULATOR_H
