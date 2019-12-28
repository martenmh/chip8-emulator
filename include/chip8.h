//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_CHIP8_EMULATOR_H
#define CHIP_8_EMULATOR_CHIP8_EMULATOR_H

#include <memory>
//#include <SDL2/SDL.h>

#include "cpu.h"
#include "memory.h"
#include "input.h"
#include "display.h"

class Chip8 {
public:
    Chip8();
    ~Chip8();

    /**
       Initialize all parts
     */
    void initialize();

    /**
       Load a game file into memory
       @param name The path name of the game
     */
    void loadGame(std::string pathName);

    Display display;
    Memory memory;
    CPU cpu;
};


#endif //CHIP_8_EMULATOR_CHIP8_EMULATOR_H
