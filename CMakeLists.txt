cmake_minimum_required(VERSION 3.0.0)
project(chip8-emulator VERSION 0.1.0)

add_executable(chip8-emulator src/main.cpp)

target_link_libraries(chip8-emulator SDL2main SDL2 GL)