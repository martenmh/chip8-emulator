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
//
Display::~Display(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Display::clearScreen() {
    pixel.setAll(PixelUnset);
}

int Display::flipPixel(int x, int y, int height){

    bool flippedToUnset = false;
    for(int n = 0; n < height; n++){
        // Check if pixel is set
        const u_char pixelStatus = pixel.at(x,y + n);
        if(pixelStatus != PixelUnset)
            flippedToUnset = true;

        pixel.at(x,y + n) = pixelStatus == PixelUnset ? PixelSet : PixelUnset;   // Flip pixel
    }

    // After flipping pixels render the result
    render();
    return flippedToUnset;
}

void Display::render() {
    // Get the magnitude of difference between the Chip-8 display and the SDL window
    const int xDiff = display_width / 64;
    const int yDiff = display_height / 32;

//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//    SDL_RenderClear(renderer);

    // Set new color for drawn pixels
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int x = 0; x < display_width; ++x)
        for(int y = 0; y < display_height; ++y)
            if(pixel.at(x / xDiff,y / yDiff) != PixelUnset)    // Keep black pixels black
                SDL_RenderDrawPoint(renderer, x, y);

    // Render result
    SDL_RenderPresent(renderer);
}

