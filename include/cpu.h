//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_CHIP8_H
#define CHIP_8_EMULATOR_CHIP8_H

class Chip8;

/**
   Data registers:
 */
#define V0 V[0]
#define V1 V[1]
#define V2 V[2]
#define V3 V[3]
#define V4 V[4]
#define V5 V[5]
#define V6 V[6]
#define V7 V[7]
#define V8 V[8]
#define V9 V[9]
#define VA V[10]
#define VB V[11]
#define VC V[12]
#define VD V[13]
#define VE V[14]
/**
   The VF register doubles as a flag
   Possible Flags:
   * carry flag         (used in addition)
   * no borrow flag     (used in subtraction)
   * pixel collision    (used when drawing)
 */
#define VF V[15]    // flag, carry flag, "no borrow" flag, carry flag

class CPU {
public:
    void initialize(Chip8 *chip8);
    void emulateCycle();

    // Opcode variable used for storing current opcode
    unsigned short opcode;

    // Registers (8 bit)
    unsigned char V[16];    // Data registers [V0...VF]
    // Registers (16 bit)
    unsigned short I;       // Address register
    unsigned short pc;      // Program counter

    // Timers (count down at 60 hertz)
    unsigned char delay_timer;  // timing events of games
    unsigned char sound_timer;  // when value != 0, make a beeping sound

    unsigned short sp;          // Stack pointer

    // Keypad 0-F
    unsigned char key[16];
    // Parent
    Chip8 *chip8_;

};


#endif //CHIP_8_EMULATOR_CHIP8_H
