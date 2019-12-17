cmake_minimum_required(VERSION 3.6)
project(chip-8-emulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

find_package(SDL2 REQUIRED)
find_package(GTest REQUIRED)

include_directories(include)


add_executable(chip-8-emulator
        include/cpu.h src/cpu.cpp           # cpu
        include/chip8.h src/chip8.cpp       # chip8
        include/memory.h src/memory.cpp     # memory
        include/display.h src/display.cpp   # display
        include/input.h src/input.cpp       # input
        include/sound.h src/sound.cpp       # sound
        src/main.cpp)                       # main

target_link_libraries(chip-8-emulator SDL2)

include_directories(${GTEST_INCLUDE_DIRS})
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests test/tests.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} gtest_main pthread)
