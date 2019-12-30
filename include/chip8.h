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

    Display *display;
    Memory memory;
    CPU cpu;
    Keyboard keyboard;

    inline void quit(){ enabled = false; }

    bool enabled;
};


#endif //CHIP_8_EMULATOR_CHIP8_EMULATOR_H
