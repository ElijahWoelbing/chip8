#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

class Chip8
{
    unsigned char memory[4095];
    // reserved for the interperter 0 - 512 unsigned chars
    int interperterStart = 0; 
    int interperterEnd = 511; 
    // where program starts
    int programMemStart = 512; 
    int memorySize = 4095; 
    // regesters
    unsigned char V[16];
    unsigned char delayTimer; 
    unsigned char soundTimer; 
    unsigned short stack[16];
    unsigned char stackPointer;
    unsigned short I;
    unsigned short programCounter;
    unsigned short opcode;
    public: unsigned char screen[64*32];
    public: unsigned char keys[16];
    public: bool draw = false;

    unsigned char fonts[80] = {  
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    public: void initialize()
    {
        programCounter = 0x200;
        opcode = 0;
        I = 0;
        stackPointer = 0;
        // clear stack, input and regesters
        for(int i = 0; i < 16; i++)
        {
            stack[i] = 0;
            keys[i] = 0;
            V[i] = 0;
        }
        //clear memory
        for(int i = 0; i < memorySize; i++)
        {
            memory[i] = 0;
        }

        //load fonts into memory
        for(int i = 0; i < 80; i++)
        {
            memory[i] = fonts[i];
        }

        delayTimer = 0;
        soundTimer = 0;
    }

    public: void loadProgram(const char* fileName)
    {
        
        std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
        
        if(file.is_open())
        {

            file.seekg(0, file.end);

            int fileLength = file.tellg();

            file.seekg(0, file.beg);

            char *buffer = new char[fileLength];

            file.read(buffer, fileLength);

            for(int i = 0; i < fileLength; i++)
            {
                
                memory[programMemStart + i] = buffer[i];
            
            }

            delete[] buffer;

        } else 
        {
            std::cout << "unable to load rom" << std::endl;
        }
    }

    public: void emulate()
    {
        opcode = memory[programCounter] << 8 | memory[programCounter + 1];
        /* nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
        n or nibble - A 4-bit value, the lowest 4 bits of the instruction
        x - A 4-bit value, the lower 4 bits of the high byte of the instruction
        y - A 4-bit value, the upper 4 bits of the low byte of the instruction
        kk or byte - An 8-bit value, the lowest 8 bits of the instruction */
        unsigned short x = (opcode & 0x0F00) >> 8;
        unsigned short y = (opcode & 0x00F0) >> 4;
        unsigned short kk = opcode & 0x00FF;
        unsigned short nnn = opcode & 0x0FFF;
        std::cout << (opcode & 0xF000) << std::endl;
        std::cout << (opcode & 0xFF) << std::endl;
        switch(opcode & 0xF000)
        {    
            case 0x0000:
            {
                switch(opcode & 0x000F)
                {
                    // 0x00E0
                    case 0x0000:
                    {
                    // Clears the screen        
                        for(int i = 0; i < 2048; i++)
                        {    
                            screen[i] = 0;                        
                        }
                        
                        draw = true;

                        programCounter += 2;

                    }
                    
                    break;
                    // 0x00EE
                    case 0x000E:
                    {
                    //decement the stack pointer and set program counter to adresss at the top of the stack  
                        programCounter = stack[--stackPointer];
                        
                        programCounter += 2;
                    
                    }
                    
                    break;

                    default:
                    {
                    
                    std::cout << "unknown opcode: " << opcode << std::endl;
                    
                    exit(3);
                    
                    }
                    
                    break;     
                }
            }
                
            break;
            //1nnn
            case 0x1000:
            {
            //  The interpreter sets the program counter to nnn.
                programCounter = nnn;
            
            }
            
            break;
            //2nnn
            case 0x2000:
            {
            //  The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
                stack[stackPointer] = programCounter;
                
                stackPointer++;

                programCounter = nnn;
            }

            break;
            // 3xkk
            case 0x3000:
            {
            //  The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
                if(V[x] == kk)
                {
                
                    programCounter += 4;
                
                }
                else
                {
                
                    programCounter += 2;
                
                }
            
            }
            
            break;
            // 4xkk
            case 0x4000:
            {
                //  The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
                if(V[x] != kk)
                {
                    
                    programCounter += 4;
                
                }
                else
                {
                    programCounter += 2;
                }
                
            }
            
            break;
            // 5xy0
            case 0x5000:
            {
            //  The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
                if(V[x] == V[y]){

                    programCounter += 4;

                } else
                {
                
                    programCounter += 2;
                
                }
            
            }
             
            break;
            // 6xkk
            case 0x6000:
            {

                V[x] = kk;
                
                programCounter += 2;

            }
            
            break;
            // 7xkk
            case 0x7000:
            {
            
                V[x] += kk;

                programCounter += 2;
            
            }
            
            break;
            
            case 0x8000: 
            {
            
                switch (opcode & 0x000F)
                {
                    // 8xy0
                    case 0x0000: 
                    {
                        
                        V[x] = V[y];
                        
                        programCounter += 2;

                    }   
                    
                    break;
                    // 8xy1
                    case 0x0001:
                    {

                        V[x] = V[x] | V[y];
                        
                        programCounter += 2;
                    
                    }
                    
                    break;
                    // 8xy2
                    case 0x0002:
                    {

                        V[x] = V[x] & V[y];
                        
                        programCounter += 2;

                    } 

                    break;
                    // 8xy3
                    case 0x0003:
                    {
                    // Set Vx = Vx XOR Vy.
                        V[x] = V[x] ^ V[y];
                        
                        programCounter += 2;

                    }
                    
                    break;
                    // 8xy4
                    case 0x0004:
                    {

                        V[x] = V[x] + V[y];

                        if(V[x] > 255)
                        {

                            V[0xF] = 1;
                            
                        } else                
                        {

                            V[0xF] = 0;

                        }
                        
                        V[x] -= 255;

                        programCounter += 2;

                    } 
                    
                    break;
                    // 8xy5
                    case 0x0005:
                    {

                        if(V[x] > V[y])
                        {

                            V[0xF] = 1;

                        } 
                        else
                        {

                            V[0xF] = 0;

                        }

                        V[x] = V[x] - V[y];

                        programCounter += 2;
                    
                    }
                    
                    break;
                    // 8xy6
                    case 0x0006:
                    {

                        V[0xF] = opcode & 0x1;
                        // divide by 2
                        V[x] = V[x] >> 1;

                        programCounter += 2;

                    }
                    
                    break;

                    case 0x0007:
                    {
                    // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
                        if(V[y] > V[x])
                        {
                            V[0xF] = 1;
                        } 
                        else
                        {
                            V[0xF] = 0;
                        }

                        V[x] = V[y] - V[x];

                        programCounter += 2;

                    }
                    
                    break;

                    case 0x000E :
                    {
                    // If the most-significant bit (before shift) of Vx is 1, then VF is set to 1, otherwise to 0
                        V[0xF] = (V[x] >> 7);
                    // Vx is multiplied by 2
                        V[x] = V[x] << 1;
                        
                        programCounter += 2;
                    
                    }
                    
                    break;
                    
                    default: 
                    {
                        
                        std::cout << "unknown opcode: " << opcode << std::endl;
                        
                        exit(3);

                    }   
                    
                    break;
                }
            
            }  
                
            break;
            // 9xy0
            case 0x9000:
            {
                
                if(V[x] != V[y])
                {
                    
                    programCounter += 4;
                
                }
                else
                {
                    programCounter += 2;
                }
            
            }                
            
            break;
            //Annn
            case 0xA000:
            {
                
                I = nnn;
                
                programCounter +=2;

            }

            break;

            case 0xB000:
            {

                programCounter = nnn + V[0];

            }
            
            break;
            
            case 0xC000:
            {
                V[x] = (rand() % 0x100) & kk;
                
                programCounter += 2;
            
            }
            
            break;
            // 0xDXYN
            case 0xD000:
            {
                // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                // The interpreter reads n bytes from memory, starting at the address stored in I. 
                // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
                // Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
                // VF is set to 1, otherwise it is set to 0. 
                //If the sprite is positioned so part of it is outside the coordinates of the display, 
                //it wraps around to the opposite side of the screen. 
                unsigned short regesterX = V[x];
                unsigned short regesterY = V[y];
                unsigned short height = opcode & 0x000F;
                unsigned char pixel;
                V[0xF] = 0;
                for (int yline = 0; yline < height; yline++)
                {
                    pixel = memory[I + yline];

                    for(int xline = 0; xline < 8; xline++)
                    {
                        if((pixel & (0x80 >> xline)) != 0)
                        {
                            if(screen[regesterX + xline + ((regesterY + yline) * 64)] == 1)
                            {
                                V[0xF] = 1;
                            }
                            screen[regesterX + xline + ((regesterY + yline) * 64)] ^= 1;
                        }
                    }
                }
    
                draw = true;
                programCounter += 2;
            }   
            
            break;
            // Ex9E
            case 0xE000:
            {
                switch (opcode & 0x00FF)
                {
                    case 0x009E:
                    {
                    // Skip next instruction if key with the value of Vx is pressed.
                    // Checks the keyboard, and if the key corresponding to the value 
                    //of Vx is currently in the down position, PC is increased by 2.

                        if(keys[V[x]] == 1)
                        {
                        
                            programCounter += 4;

                        }
                        else
                        {
                            
                            programCounter += 2;

                        }
                        
                    
                    }
                    
                    break;

                    case 0x00A1:
                    {
                    // Skip next instruction if key with the value of Vx is not pressed.
                    // Checks the keyboard, and if the key corresponding to the value of
                    // Vx is currently in the up position, PC is increased by 2.
                        if(keys[V[x]] == 0)
                        {

                            programCounter += 4;

                        }
                        else
                        {
                            
                            programCounter += 2;

                        }
                        
                    }
                    
                    break;

                    default:
                    {

                        std::cout << "unknown opcode: " << opcode << std::endl;

                        exit(EXIT_FAILURE);
                    }
                    
                    break;

                }

            }
            
            break;

            case 0xF000 :
            {
                switch (opcode & 0xFF)
                {
                    // Fx07
                    case 0x7:
                    {
                    // The value of DT is placed into Vx.
                        V[x] = delayTimer;

                        programCounter += 2;

                    }

                    break;
                    // Fx0A
                    case 0xA:
                    {
                    // Wait for a key press, store the value of the key in Vx.
                        bool keyPressed = false;

                        for(int i = 0; i < 16; i++){
                            if(keys[i] == 1)
                            {
                                V[x] = i;

                                keyPressed = true;

                            }
                        }

                        if(!keyPressed)
                        {
                            return;
                        }
                        else
                        {
                            programCounter += 2;
                        }
                        
                    }

                    break;
                    // Fx15
                    case 0x15:
                    {
                    // Set delay timer = Vx.
                        delayTimer = V[x];

                        programCounter += 2;

                    }

                    break;
                    // Fx18 
                    case 0x18:
                    {
                    // Set sound timer = Vx.   
                        soundTimer = V[x];

                        programCounter += 2;

                    }    

                    break;
                    // Fx1E
                    case 0x1E:
                    {
                        // Set I = I + Vx.
                        if(I + V[x] > 0xFFF)
                            V[0xF] = 1;
                        else
                        {
                            V[0xF] = 0;

                        }
                        
                        I += V[x];

                        programCounter += 2;

                    }    

                    break; 
                    // Fx29
                    case 0x29:
                    {
                    // The value of I is set to the location for the hexadecimal sprite corresponding 
                    // to the value of Vx. See section 2.4, Display, for more information on the Chip-8 
                    // hexadecimal font.
                        I = V[x] * 5;

                        programCounter += 2;

                    }

                    break;
                    // Fx33
                    case 0x33:
                    {
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    // The interpreter takes the decimal value of Vx, and places the hundreds 
                    // digit in memory at location in I, the tens digit at location I+1, and 
                    // the ones digit at location I+2.
                        memory[I] = (V[x] / 100) % 100;

                        memory[I + 1] = (V[x] / 10) % 10;

                        memory[I + 2] = V[x] % 10;

                        programCounter += 2;

                    }    

                    break;
                    // Fx55 
                    case 0x55:
                    {
                    // Store registers V0 through Vx in memory starting at location I.
                    // The interpreter copies the values of registers V0 through Vx into
                    //  memory, starting at the address in I.
                        for(int i = 0; i <= x; i++)
                        {
                            memory[I + i] = V[i];
                        }
                    
                        I = I + x + 1;
                        
                        programCounter += 2;

                    }

                    break;
                    // Fx65 
                    case 0x65:
                    {
                    // Read registers V0 through Vx from memory starting at location I.
                    // The interpreter reads values from memory starting at location I 
                    // into registers V0 through Vx.
                        for(int i = 0; i <= x; i++)
                        {
                            V[i] = memory[I + i];
                        }
                        
                        I = I + x + 1;

                        programCounter += 2;
                    }

                    break;                    

                    default:
                    {

                        std::cout << "unknown opcode: " << opcode << std::endl;

                        exit(EXIT_FAILURE);
                    }   

                    break;
                }
            }   
            
            break;

            default:
            {
                
                std::cout << "unknown opcode: " << opcode << std::endl;
                
                exit(3);
            
            }
            
            break;
        }
        
        if(delayTimer > 0){
         
            --delayTimer;
       
        }
         
        if(soundTimer > 0)
        {
            if(soundTimer == 1)
            {
             
                printf("BEEP!\n");
             
                --soundTimer;
         
            }
        }       
        
    }
};
