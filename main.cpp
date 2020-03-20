#include <iostream>
#include "Timer.cpp"

unsigned char memory[4096];
// reserved for the interperter 0 - 512 unsigned chars
unsigned char* interperterStart = &memory[0x000]; // 0
unsigned char* interperterEnd = &memory[0x1FF]; // 511
// where program starts
unsigned char* programMemStart = &memory[0x200]; // 512
unsigned char* ETI_660_programMemStart = &memory[0x600]; // 1536
unsigned char* programMemEnd = &memory[0xFFF]; // 4095
//x * (16^2) x *(16^1) x* (16^0)

// regesters
unsigned char V0;
unsigned char V1;
unsigned char V2;
unsigned char V3;
unsigned char V4;
unsigned char V5;
unsigned char V6;
unsigned char V7;
unsigned char V8;
unsigned char V9;
unsigned char VA;
unsigned char VB;
unsigned char VC;
unsigned char VD;
unsigned char VE;
unsigned char VF; // used as a flag by some instruction
unsigned char D; // delay timer
unsigned char ST; // sound timer
unsigned short stack[16];
unsigned char stackPointer;
unsigned short I;// generally used to store memory addresses
unsigned short programCounter;
unsigned char screen[32][64];
unsigned char key[16];
int main()
{
    std:: cout << sizeof(short);
    //game loop
    while(true)
    {
        break;
    }
}