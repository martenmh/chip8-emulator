#include <iostream>
#include "chip8.h"
#include <string>
#include <fstream>

void output(std::ostream &os = std::cout, std::string msg = ""){
    os << msg << std::endl;
}

int main(int argc, char *argv[]){
    bool verbose{false}, debug{false};
    // Default program PONG
    std::string programPath = "res/PONG";

    std::string usage = "Usage: ";
    (usage += argv[0]) += " [-option] [path/to/program to emulate]";
    usage += "\nOptions:\n"
             "\t-v,--verbose Specify if you want to have any console output from the program\n"
             "\t-h,--help\t\tShow the available commands";
    if(argc > 4) {
        output(std::cerr, usage);
        return 1;
    } else if(argc >= 2 && argc <= 3) {
        for(int i = 1; i < argc; i++){
            std::string arg = argv[i];
            if(arg == "-v" || arg == "--verbose") {
                verbose = true;
            } else if(arg == "-h" || arg == "--help"){
                output(std::cout,"Options:\n"
                                 "\t-v, --verbose\tSpecify if you want to have any console output from the program\n"
                                 "\t-d, --debug\tRun emulator in debug mode, this lets you run through each instruction individually while seeing the keypad, disassembly & register info\n"
                                 "\t-h, --help\tShow the available commands"
                );
                return 0;
            } else if(arg == "-d" || arg == "--debug"){
                debug = true;
            } else if(arg[0] != '-'){
                programPath = arg;
            } else{
                output(std::cerr, "Unknown option " + arg + '\n' + usage);
                return 1;
            }
        }
    }


    if(verbose)
        std::cout << "Initializing.." << std::endl;
    Chip8 chip8 = Chip8();

    if(verbose)
        std::cout << "Loading game " << std::endl;

    chip8.loadProgram(programPath);

    // Get CPU information

    //chip8.cpu.getDisassembly(std::cout);

    while(chip8.isRunning()){
        // If in debug mode wait until the user presses enter for the next instruction
        if(debug) {
            chip8.keyboard.waitForKeyPress(SDL_SCANCODE_RETURN);
        }

        if(!chip8.isRunning()) break;

        if(verbose || debug)
            std::cout << chip8.cpu.pc << " ";

        chip8.keyboard.pollKeyPad();
        chip8.cpu.emulateCycle();

        // Write current assembly
        if(verbose || debug) {
            CPU::getOpcode(chip8.cpu.opcode, std::cout);
        } if(debug) {
            chip8.cpu.getCPUInfo(std::cout);
            chip8.keyboard.getKeyboardInfo(std::cout, 15);
        }
        if(debug)
            SDL_Delay(150);
    }
    if(verbose)
        std::cout << "Stopping emulator.." << std::endl;
    return 0;
}
