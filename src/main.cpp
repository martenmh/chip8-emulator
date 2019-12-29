#include <iostream>
#include "chip8.h"
#include <string>
#include <fstream>

void output(std::ostream &os = std::cout, std::string msg = ""){
    os << msg << std::endl;
}

int main(int argc, char *argv[]){
    if(argc < 1) {
        std::string usage = "Usage: ";
        (usage += argv[0]) += " [-options] [arguments..]";
        output(std::cerr, usage);
    } else {
        std::string arg = argv[2];
        if(arg == "-v") {
            output(std::cout, "");
        } else if(arg == "-h" || arg == "--help"){
            output(std::cout,"Options\n"
                             "\t-h,--help\t\tShow this help message\n"
                             "\t-d,--destination DESTINATION\tSpecify the destination path");
            return 0;
        }
    }

    Chip8 chip8;
    std::cout << "initializing" << std::endl;
    chip8.initialize();
    std::cout << "loading game" << std::endl;
    chip8.loadProgram("res/tic-tac-toe.ch8");
    std::cout << "starting cpu" << std::endl;


    // Get CPU information
    // chip8.cpu.getCPUInfo(std::cout);
    chip8.cpu.getDisassembly(std::cout);


    std::cout << "waiting" << std::endl;
    for(auto r : {1,2,3}){
        chip8.display->flipPixel(r * 3,r* 3,3 * r);
        SDL_Delay(3000);

    }
//    for(;;){
//        chip8.cpu.emulateCycle();
//    }
    return 0;
}
