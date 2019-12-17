//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_DISPLAY_H
#define CHIP_8_EMULATOR_DISPLAY_H

#include <SDL2/SDL.h>
#include <string>

class Chip8_display {
public:
    Chip8_display(std::string title, int x, int y, int w, int h, uint flags);
    Chip8_display();
    Chip8_display(Chip8_display *pDisplay);
    ~Chip8_display();

private:
    SDL_Window *window;
};


#endif //CHIP_8_EMULATOR_DISPLAY_H
