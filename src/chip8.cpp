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
    SDL_Quit();
}

void Chip8::initialize() {
    cpu.initialize();
}

void Chip8::loadGame(std::string pathName) {
    std::ifstream file(pathName);
    if(file.bad())
        std::cerr << "Failed loading game into memory" << std::endl;

    // Load game to 0xEFF (max)
    for(int i = 0; i < 0xEFF; i++)
        memory[i + 0x200] = file.get(); // Start at 0x200

}   // file.close();
