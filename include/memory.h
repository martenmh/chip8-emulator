//
// Created by marten on 17-12-19.
//

#ifndef CHIP_8_EMULATOR_MEMORY_H
#define CHIP_8_EMULATOR_MEMORY_H


class Memory {
public:
    constexpr unsigned char& operator[](int index){ return memory[index]; }
    // 4kb of bytes (= unsigned char) 0x000-0x1000
    unsigned char memory[4096];

    // Stack to store return addresses when subroutines are called (call stack)
    unsigned short stack[16];   // Call stack
};


#endif //CHIP_8_EMULATOR_MEMORY_H
