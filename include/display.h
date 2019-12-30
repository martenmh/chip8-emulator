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
        // Bound checking
        if(x < 0 || x > 64)
            std::cerr << "Pixel (" << x << ", " << y << ") out of bounds, please give x a value between 0 and 64" << std::endl;
        if(y < 0 || y > 32)
            std::cerr << "Pixel(" << x << ", " << y << ") out of bounds, please give y a value between 0 and 32" << std::endl;
        return gfc[x][y];
    }
};

enum PixelStatus{
    PixelSet = 255, // White
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
    //int flipPixel(int x, int y, int height);
    int flipPixel(unsigned short x, unsigned short y, unsigned short height);

    /**
       Go through all pixels and render them with the appropriate color
     */
    //void render();
    ~Display();
    void clearScreen();
    void render();
    void render2();
    // Graphics
    Pixel pixel;
    SDL_Window *window;
    SDL_Renderer *renderer;

private:
    unsigned char gfx[64 * 32]; // Display resolution of 64 x 32 pixels;
    Chip8 *chip8_;
    int display_width;
    int display_height;


};


#endif //CHIP_8_EMULATOR_DISPLAY_H
