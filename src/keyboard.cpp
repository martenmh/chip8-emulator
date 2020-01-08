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

#include <iomanip>  // Formatting, setw(), left
using namespace std;

void Keyboard::getKeyboardInfo(std::ostream &os, unsigned int width) {
    //  Keypad
    //  +-+-+-+-+
    //  |1|2|3|C|
    //  +-+-+-+-+
    //  |4|5|6|D|
    //  +-+-+-+-+
    //  |7|8|9|E|
    //  +-+-+-+-+
    //  |A|0|B|F|
    //  +-+-+-+-+
    os << setw(width) << (keypad[1] ? "1 (pressed)" : "1") << setw(width) << (keypad[2] ? "2 (pressed)" : "2") << setw(width) << (keypad[3] ? "3 (pressed)" : "3") << setw(width) << (keypad[12] ? "C (pressed)" : "C") << endl;
    os << setw(width) << (keypad[4] ? "4 (pressed)" : "4") << setw(width) << (keypad[5] ? "5 (pressed)" : "5") << setw(width) << (keypad[6] ? "6 (pressed)" : "6") << setw(width) << (keypad[13] ? "D (pressed)" : "D") << endl;
    os << setw(width) << (keypad[7] ? "7 (pressed)" : "7") << setw(width) << (keypad[8] ? "8 (pressed)" : "8") << setw(width) << (keypad[9] ? "9 (pressed)" : "9") << setw(width) << (keypad[14] ? "E (pressed)" : "E") << endl;
    os << setw(width) << (keypad[10] ? "A (pressed)" : "A") << setw(width) << (keypad[0] ? "0 (pressed)" : "0") << setw(width) << (keypad[11] ? "B (pressed)" : "B") << setw(width) << (keypad[15] ? "F (pressed)" : "F") << '\n' << endl;
}

unsigned short Keyboard::waitForKeyPress(SDL_Scancode key1, SDL_Scancode key2, SDL_Scancode key3) {
    while(chip8_->isRunning()){
        // Get current pressed keys
        pollKeyPad();
        // If key is the pressed key break out of the loop or if the
        for(auto key : {key1, key2, key3}){
            if(keys[key])
                return key;
        }

    }
}
