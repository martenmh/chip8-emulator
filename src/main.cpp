#include <iostream>
#include "chip8.h"
#include <string>
#include <fstream>

void output(std::ostream &os = std::cout, std::string msg = ""){
    os << msg << std::endl;
}

int main(int argc, char *argv[]){
    bool verbose{false}, debug{false};

    if(argc > 2) {
        std::string usage = "Usage: ";
        (usage += argv[0]) += " [-option]";
        usage += "\nOptions:\n"
                                "\t-v,--verbose Specify if you want to have any console output from the program\n"
                                "\t-h,--help\t\tShow the available commands";
        output(std::cerr, usage);
        return 1;
    } else if(argc == 2) {
        std::string arg = argv[1];
        if(arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if(arg == "-h" || arg == "--help"){
            output(std::cout,"Options:\n"
                             "\t-v,--verbose Specify if you want to have any console output from the program\n"
                             "\t-h,--help\t\tShow the available commands"
                             );
            return 0;
        } else if(arg == "-d" || arg == "--debug"){
            debug = true;
        }
    }


    if(verbose)
        std::cout << "Initializing.." << std::endl;
    Chip8 chip8 = Chip8();

    if(verbose)
        std::cout << "Loading game tic-tac-toe.." << std::endl;

    chip8.loadProgram("res/Airplane.ch8");

    // Get CPU information

    //chip8.cpu.getDisassembly(std::cout);

    while(chip8.isRunning()){
        // If in debug mode wait until the user presses enter for the next instruction
        if(debug)
            chip8.keyboard.waitForKeyPress(SDL_SCANCODE_RETURN);

        if(!chip8.isRunning()) break;

        chip8.keyboard.pollKeyPad();
        chip8.cpu.emulateCycle();
        // Write current assembly
        chip8.cpu.getOpcode(chip8.cpu.opcode, std::cout);
        chip8.cpu.getCPUInfo(std::cout);
    }
    if(verbose)
        std::cout << "Stopping emulator.." << std::endl;
    return 0;
}
