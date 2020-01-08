//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_CHIP8_EMULATOR_H
#define CHIP_8_EMULATOR_CHIP8_EMULATOR_H

#include <memory>
#include <SDL2/SDL.h>
#include <stack>

#include "cpu.h"
#include "memory.h"
#include "keyboard.h"
#include "display.h"
#include "keyboard.h"

typedef unsigned short opcode;

enum ExitCode{
    NO_ERROR = 0,
    UNKNOWN_OPCODE = 1
};

class Chip8 {
public:
    Chip8();
    ~Chip8();

    /**
       Load a program file into memory
       @param name The path name of the game
     */
    void getMemory(unsigned short from, unsigned short to, std::ostream &os = std::cout);
    void loadProgram(std::string pathName);
    bool loadProgram2(const char *file_path);
    inline bool isRunning(){ return enabled; }
    void loadFontset();
    // The various Chip8 parts

    // Memory
    // 4kb of bytes (= unsigned char) 0x000-0x1000
    unsigned char memory[4096];
    // Stack to store return addresses when subroutines are called (call stack)
    unsigned short stack[16];   // Call stack

    CPU cpu;
    Display *display;   // Output
    Keyboard keyboard;  // Input

    inline void quit(int exit = NO_ERROR, const std::string reason = ""){
        enabled = false;
        exitCode = static_cast<ExitCode>(exit);
        if(exit != 0)
            std::cerr << reason << std::endl;
    }
    ExitCode exitCode;
    inline ExitCode getExitCode(){ return exitCode; }

    static unsigned short getAddressOfChar(unsigned char c);

    static std::string hexToReadableOpcode(unsigned short opcode);
    unsigned char fontset[80] =
    {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    bool drawFlag;
private:
    bool reverse;

    bool enabled;
    bool paused;
};


#endif //CHIP_8_EMULATOR_CHIP8_EMULATOR_H
