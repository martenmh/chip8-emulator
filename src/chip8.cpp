//
// Created by marten on 17-12-19.
//

#include "chip8.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

Chip8::Chip8(): enabled{true}, paused{false}, reverse{false}{
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
    loadFontset();
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
        memory[i + 0x200] = c; // Start at 0x200
        // Increment after usage
        i++;
    }
    memory[i + 1] = '\0';
}

unsigned short Chip8::getAddressOfChar(unsigned char c){
    switch(c) {
        case 0: return 0x50 + 0;
        case 1: return 0x50 + 5;
        case 2: return 0x50 + 10;
        case 3: return 0x50 + 15;
        case 4: return 0x50 + 20;
        case 5: return 0x50 + 25;
        case 6: return 0x50 + 30;
        case 7: return 0x50 + 35;
        case 8: return 0x50 + 40;
        case 9: return 0x50 + 45;
        case 10: return 0x50 + 50;
        case 11: return 0x50 + 55;
        case 12: return 0x50 + 60;
        case 13: return 0x50 + 65;
        case 14: return 0x50 + 70;
        case 15: return 0x50 + 75;
        default: return 0x50 + 200;
    }
}

void Chip8::loadFontset() {
    // Load font data into 0x000-0x200
    for(int i = 0; i <= 80; i++){
        memory[i + 0x50] = fontset[i];
    }
}

std::string Chip8::hexToReadableOpcode(unsigned short opcode) {
    const char firstNibble = (opcode & 0xF000) >> 12;
    const char secondNibble = (opcode & 0x0F00) >> 8;
    const char thirdNibble = (opcode & 0x00F0) >> 4;
    const char fourthNibble = (opcode & 0x000F);
    // Convert Hex 0-F to character 0-F
    auto checkNibble = [](char f){return f > 9 ? (char)(f + 55) : (char)(f + 48); };
    return std::string({checkNibble(firstNibble), checkNibble(secondNibble), checkNibble(thirdNibble), checkNibble(fourthNibble)});
}

void Chip8::undo() {

}

void Chip8::getMemory(unsigned short from, unsigned short to, std::ostream &os) {
    for(unsigned short i = from; i <= to; i++){
        os << (int)memory[i] << " ";
    }
}
// file.close()
