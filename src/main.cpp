#include <iostream>
//#include "SDL2/SDL.h"
#include "chip8.h"

int main(){
    Chip8 chip8;
    std::cout << "initializing" << std::endl;
    chip8.initialize();
    std::cout << "loading game" << std::endl;
    chip8.loadGame("res/tic-tac-toe.ch8");
    std::cout << "starting cpu" << std::endl;

    for(;;){
        chip8.cpu.emulateCycle();
    }
    return 0;
}
