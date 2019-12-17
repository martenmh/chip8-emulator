//
// Created by marten on 17-12-19.
//
#include <iostream>
#include "chip8_display.h"

Chip8_display::Chip8_display(std::string title, int x, int y, int w, int h, uint flags){
    window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

    if(window == NULL)
        std::cerr << "An error occurred while creating the window" << std::endl;
}

Chip8_display::Chip8_display(){

}

Chip8_display::Chip8_display(Chip8_display *pDisplay){
    window = pDisplay->window;
}

Chip8_display::~Chip8_display(){
    SDL_DestroyWindow(window);
}