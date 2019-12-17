//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_DISPLAY_H
#define CHIP_8_EMULATOR_DISPLAY_H

#include <SDL2/SDL.h>
#include <string>

class Display {
public:
    Display(std::string title, int x, int y, int w, int h, uint flags);
    Display();
    Display(Display *pDisplay);
    ~Display();

    // Graphics
    unsigned char gfc[64 * 32]; // Display resolution of 64 x 32 pixels;

private:
    SDL_Window *window;
};


#endif //CHIP_8_EMULATOR_DISPLAY_H
