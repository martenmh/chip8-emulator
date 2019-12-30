//
// Created by marten on 17-12-19.
//

#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

Chip8::Chip8(): enabled{true}{
    cpu = CPU(this);

    // Initialize display
    display = new Display(this,
                          "An SDL2 Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640,
                          320,
                          SDL_WINDOW_OPENGL);

    keyboard = Keyboard(this);
}

Chip8::~Chip8(){
    if(enabled)
        enabled = false;

    // TODO: Use smart pointers
    delete display;
    SDL_Quit();
}



void Chip8::loadProgram(std::string pathName) {
    std::ifstream file(pathName);
    if(file.bad())
        std::cerr << "Failed loading game into memory" << std::endl;

    int i = 0;
    // Load game up to 0xEFF or EOF
    while(!file.eof() && i < 0xEFF - 0x001 ) {

        char c = file.get();
        // Skip spaces & enters
        if(c == ' ' || c == '\n')
            continue;

        // Write to memory address
        memory[i++ + 0x200] = c; // Start at 0x200
        // Increment after usage
    }
    memory[i + 1] = '\0';
}   // file.close()
