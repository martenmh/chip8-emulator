//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_DISPLAY_H
#define CHIP_8_EMULATOR_DISPLAY_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

struct Pixel{
    unsigned char gfc[64][32]; // Display resolution of 64 x 32 pixels;

    /**
       Set all values in pixel matrix
     * @param value to set all pixel to
     */
    inline void setAll(u_char value){
        for(int x = 0; x < 64; x++)
            for(int y = 0; y < 32; y++)
                gfc[x][y] = value;
    }

    /**
       Get a pixel reference at a given (x,y) coordinate
     */
    inline u_char& at(int x, int y){
        // Pixels have to go to the other side when going out of bounds
        while(x > 63){ x -= 64; }
        while(x < 0){ x += 64; }
        while(y > 31){ y -= 32; }
        while(y < 0){ y += 32; }

        return gfc[x][y];
    }
};

enum PixelStatus{
    PixelSet = 1, // White
    PixelUnset = 0  // Black
};

class Chip8;

class Display {
public:
    Display(Chip8 *parent, std::string title, int x, int y, int w, int h, uint flags = 1);
    Display();
    Display(Display *pDisplay);

    /**
       Draw sprite at coordinate (x, y) with height of N pixels
       @return 1 if any screen pixels are flipped from set to unset, and to 0 if that doesn't happen
     */
    int display(unsigned short x, unsigned short y, unsigned short height);

    /**
       Go through all pixels and render them with the appropriate color
     */
    void render();
    ~Display();
    void clearScreen();

    // Graphics
    Pixel pixel;
    SDL_Window *window;
    SDL_Renderer *renderer;

private:
    Chip8 *chip8_;
    int display_width;
    int display_height;


};


#endif //CHIP_8_EMULATOR_DISPLAY_H
