//
// Created by marten on 17-12-19.
//

#include "chip8.h"

#include <iostream>
#include <fstream>

Chip8::Chip8(){
//    SDL_Init(SDL_INIT_VIDEO);
//
//    display = std::make_unique<Display>(new Display(
//            "An SDL2 Window",
//            SDL_WINDOWPOS_UNDEFINED,
//            SDL_WINDOWPOS_UNDEFINED,
//            640,
//            480,
//            SDL_WINDOW_OPENGL));
//    SDL_Delay(3000);
}

Chip8::~Chip8(){
    //SDL_Quit();
}

void Chip8::initialize() {
    cpu.initialize(this);
}

void Chip8::loadGame(std::string pathName) {
    std::ifstream file(pathName);
    if(file.bad())
        std::cerr << "Failed loading game into memory" << std::endl;

    int i = 0;
    // Load game up to 0xEFF or EOF
    while(!file.eof() && i < 0xEFF ) {
        // Skip spaces & enters
        char c = file.get();
        if(c == ' ' || c == '\n')
            continue;

        // Write to memory address
        memory[i++ + 0x200] = c; // Start at 0x200
        // Increment after usage
    }
}   // file.close()
