#include <iostream>
#include "SDL2/SDL.h"
#include "chip8.h"

int main(){
    Chip8 chip8;
    chip8.initialize();
    // chip8.loadGame("res/tic-tac-toe.ch8");
    return 0;
}
