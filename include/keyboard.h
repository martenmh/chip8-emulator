//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_KEYBOARD_H
#define CHIP_8_EMULATOR_KEYBOARD_H

#include <SDL2/SDL.h>
#include <ostream>
#include <iostream>

enum key {
    // Chip8 keypad buttons
    A   = 4,
    B   = 5,
    C   = 6,
    D   = 7,
    E   = 8,
    F   = 9,
    N1  = 30,
    N2  = 31,
    N3  = 32,
    N4  = 33,
    N5  = 34,
    N6  = 35,
    N7  = 36,
    N8  = 37,
    N9  = 38,
    N0  = 39,
    // Buttons to stop Chip8
    ESCAPE  = 41,
    BACKSPACE = 42,
    LCTRL = 224,
};

class Chip8;

//  Copied from http://www.multigesture.net
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
class Keyboard {
public:
    explicit Keyboard(Chip8 *parent);
    Keyboard();

    /**
       Wait for a key to be pressed and halt further CPU instructions
       @return The key that has been pressed
     */
    unsigned char waitForKeyPress();
    void waitForKeyPress(unsigned short key);
    /**
       Check all pressed keys (used in each clock cycle)
     */
    void pollKeyPad();

    /* Check if a key is pressed */
    inline bool keyIsPressed(unsigned char key){ return keypad[key]; }


    void getKeyboardInfo(std::ostream &os = std::cout, unsigned int width = 23);

private:
    Chip8 *chip8_;
    const Uint8 *keys;
    SDL_Event e;

    // Keypad 0-F
    unsigned char keypad[16];
};


#endif //CHIP_8_EMULATOR_KEYBOARD_H
