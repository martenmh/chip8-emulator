//
// Created by marten on 17-12-19.
//

#include "keyboard.h"
#include "chip8.h"

Keyboard::Keyboard(Chip8 *parent): chip8_{parent}{
    keys = SDL_GetKeyboardState(NULL);
}

void Keyboard::pollKeyPad() {
    // Set pressed buttons to keys by polling SDL
    SDL_PollEvent(&e);

    // If ESCAPE, BACKSPACE or CTRL+C is pressed, quit emulator
    if(keys[ESCAPE] || keys[BACKSPACE] || (keys[LCTRL] && keys[C]))
        return chip8_->quit();

    int i = 0;
    // Check keys 0-F
    for(int keyCheck : {N0,N1,N2,N3,N4,N5,N6,N7,N8,N9, A,B,C,D,E,F})
        keypad[i++] = keys[keyCheck];   // Set pressed buttons of keypad

}

Keyboard::Keyboard() {

}

unsigned char Keyboard::waitForKeyPress() {
    while(chip8_->isRunning()){
        // Get current pressed keys
        pollKeyPad();
        // If any key is pressed return it as the pressed key
        for(auto key : keypad )
            if(key != 0)
                return key;
    }
}

void Keyboard::waitForKeyPress(unsigned short key) {
    while(chip8_->isRunning()){
        // Get current pressed keys
        pollKeyPad();
        // If key is the pressed key break out of the loop or if the
        if(keys[key])
            return;
    }
}
