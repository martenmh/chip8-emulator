////
//// Created by marten on 17-12-19.
////
#include "display.h"
#include <chip8.h>

Display::Display(Chip8 *parent, std::string title, int x, int y, int w, int h, uint flags): display_height{h}, display_width{w}, chip8_{parent} {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Event event;

    SDL_CreateWindowAndRenderer(800, 600, flags, &window, &renderer);

    if (window == NULL)
        std::cerr << "An error occurred while creating the window" << std::endl;

    clearScreen();
    render();
}

Display::Display() : display_height{0}, display_width{0} {

}

Display::Display(Display *pDisplay) : display_height{pDisplay->display_height}, display_width{pDisplay->display_width}{
    window = pDisplay->window;
}

Display::~Display(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Display::clearScreen() {
    pixel.setAll(PixelUnset);
}

void Display::render() {
    // Set the drawFlag back to false
    chip8_->drawFlag = false;

    // Get the magnitude of difference between the Chip-8 display and the SDL window
    const int xDiff = display_width / 64;
    const int yDiff = display_height / 32;

    // For each pixel draw black or white
    for (int x = 0; x < display_width; ++x)
        for(int y = 0; y < display_height; ++y)
            if(pixel.at(x / xDiff,y / yDiff) != PixelUnset) {
                // Set new color for drawn pixels
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            } else{
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer, x, y);
            }

    // Render result
    SDL_RenderPresent(renderer);
}

/**
  Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
  Each row of 8 pixels is read as bit-coded starting from memory location I;
  I value doesn’t change after the execution of this instruction.
  As described above, VF is set to 1 if any screen pixels are
  flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
  @param x
  @param y
  @param height
  @return
 */
int Display::display(unsigned short x, unsigned short y, unsigned short n) {
    // Check if n is between 1 and 15
    if(n < 1 || n > 15)
        std::cerr << "Please give a height between 1 and 15" << std::endl;

    int flippedToUnset = 0;
    for(int height = 0; height < n; height++){
        // Get the 8 bits from memory
        unsigned char spriteRow = chip8_->memory[chip8_->cpu.I + height];

        for(int width = 8; width > 0; width--){
            // Get a single bit from the row
            char spritePixel = (spriteRow >> (width > 0 ? width - 1 : 0)) & 1;
            int newX = x + (8 - width);
            int newY = y + height;

            auto &pixels = pixel.at(newX, newY);
            int pixelVal = (int)pixels;

            pixels ^= spritePixel;
            if(pixelVal == 1 && pixels == 0)
                flippedToUnset = 1;
        }
    }

    return flippedToUnset;
}

