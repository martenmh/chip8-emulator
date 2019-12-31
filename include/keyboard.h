//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_KEYBOARD_H
#define CHIP_8_EMULATOR_KEYBOARD_H

#include <SDL2/SDL.h>
#include <ostream>
#include <iostream>
#include <array>


//SDL_SCANCODE_A = 4,
//SDL_SCANCODE_B = 5,
//SDL_SCANCODE_C = 6,
//SDL_SCANCODE_D = 7,
//SDL_SCANCODE_E = 8,
//SDL_SCANCODE_F = 9,
//SDL_SCANCODE_G = 10,
//SDL_SCANCODE_H = 11,
//SDL_SCANCODE_I = 12,
//SDL_SCANCODE_J = 13,
//SDL_SCANCODE_K = 14,
//SDL_SCANCODE_L = 15,
//SDL_SCANCODE_M = 16,
//SDL_SCANCODE_N = 17,
//SDL_SCANCODE_O = 18,
//SDL_SCANCODE_P = 19,
//SDL_SCANCODE_Q = 20,
//SDL_SCANCODE_R = 21,
//SDL_SCANCODE_S = 22,
//SDL_SCANCODE_T = 23,
//SDL_SCANCODE_U = 24,
//SDL_SCANCODE_V = 25,
//SDL_SCANCODE_W = 26,
//SDL_SCANCODE_X = 27,
//SDL_SCANCODE_Y = 28,
//SDL_SCANCODE_Z = 29,
//
//SDL_SCANCODE_1 = 30,
//SDL_SCANCODE_2 = 31,
//SDL_SCANCODE_3 = 32,
//SDL_SCANCODE_4 = 33,
//SDL_SCANCODE_5 = 34,
//SDL_SCANCODE_6 = 35,
//SDL_SCANCODE_7 = 36,
//SDL_SCANCODE_8 = 37,
//SDL_SCANCODE_9 = 38,
//SDL_SCANCODE_0 = 39,

enum Key {
    // Chip8 keypad buttons
    /*
     * 1 2 3 C              1 2 3 4
     * 4 5 6 D      =>      Q W E R
     * 7 8 9 E              A S D F
     * A 0 B F              Z X C V
     */
    N1  = 30, N2  = 31, N3  = 32, C   = 33,
    N4  = 20, N5  = 26, N6  = 8, D  = 21,
    N7 = 4, N8 = 22, N9 = 7, E = 9,
    A = 29, N0 = 27, B = 6, F = 25,

    // Buttons to stop Chip8
    ESCAPE  = 41,
    BACKSPACE = 42,
    LCTRL = 224,

    // Debugging
    RETURN = 40,    // Next
    RIGHT = 79,     // Next
    LEFT = 80       // Prev

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

    unsigned short waitForKeyPress(SDL_Scancode key1, SDL_Scancode key2, SDL_Scancode key3);

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
