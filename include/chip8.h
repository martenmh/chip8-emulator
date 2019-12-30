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
    Memory memory;
    CPU cpu;
    Display *display;   // Output
    Keyboard keyboard;  // Input

    inline void quit(){ enabled = false; }

private:
    bool enabled;
};


#endif //CHIP_8_EMULATOR_CHIP8_EMULATOR_H
