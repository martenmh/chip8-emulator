//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_CHIP8_H
#define CHIP_8_EMULATOR_CHIP8_H

#include <ostream>
#include <vector>
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

/**
   Convert a decimal to a 12 bit (3 * 4 bit) binary coded decimal (up to 999)
   Each number in the decimal will be converted to 4 bits and put into the bcd
   Example  365
   Binary:  0001 0110 0101
   BCD:     0011 0101 0111
            3    6    5
   @return The binary coded decimal
 */
static std::vector<unsigned char> binaryCodedDecimal(unsigned short int decimal);


class CPU {
public:
    CPU(Chip8 *chip8);
    CPU();
    void initialize(Chip8 *chip8);

    /* All instructions are performed here */
    void emulateCycle();

    /**
       Get complete disassembly of loaded program
       @param os stream to give output to
       TODO: Create GUI for disassembly
     */
    void getDisassembly(std::ostream &os);
    /**
       Get all info from the CPU, including registers and timers
       @param os stream to give output to
       TODO: Create GUI for CPU info
     */
    void getCPUInfo(std::ostream &os);

    // Opcode variable used for storing current opcode
    unsigned short opcode;
;
    // Registers (8 bit)
    unsigned char V[16];    // Data registers [V0...VF]
    // Registers (16 bit)
    unsigned short I;       // Address register
    unsigned short pc;      // Program counter

    // Timers (count down at 60 hertz)
    unsigned char delay_timer;  // timing events of games
    unsigned char sound_timer;  // when value != 0, make a beeping sound

    unsigned short sp;          // Stack pointer

    void getOpcode(unsigned short opcode, std::ostream &os);

private:
    // Parent
    Chip8 *chip8_;

};


#endif //CHIP_8_EMULATOR_CHIP8_H
