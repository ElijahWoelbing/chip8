
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
    unsigned char screen[32 * 64];
    unsigned char key[16];

    public: Chip8()
    {

    }

    public: void initialize(){
        programCounter = 0x200;
        opcode = 0;
        I = 0;
        stackPointer = 0;
        // clear stack, input and regesters
        for(int i = 0; i < 16; i++)
        {
            stack[i] = 0;
            key[i] = 0;
            V[i] = 0;
        }
        //clear memory
        for(int i = 0; i < memorySize; i++)
        {
            memory[i] = 0;
        }

        delayTimer = 0;
        soundTimer = 0;
    }

    public: void loadProgram()
    {
        for(int i = 0; i < programSize; i++)
        {
            memory[programMemStart + i] = program[i];
        }
    }

    public: void emulate()
    {
        opcode = memory[programCounter] << 8 | memory[programCounter + 1];
        
        unsigned short x = (opcode & 0x0F00) >> 8;
        unsigned short y = (opcode & 0x00F0) >> 4;
        unsigned short kk = opcode & 0x00FF;
        unsigned short nnn = opcode & 0x0FFF;
        
        switch(opcode & 0xF000)
        {    
            case 0x0000:
                switch(opcode & 0x000F)
                {
                    case 0x0000: // 0x00E0: Clears the screen        
                    // Execute opcode
                    break;

                    case 0x000E: // 0x00EE: Returns from subroutine          
                    // Execute opcode
                    break;

                    default:
                    printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
                }
            
                break;
            //1nnn
            case 0x1000:
            
                programCounter = nnn;
            
            break;
            //2nnn
            case 0x2000:
                
                stackPointer++;

                stack[stackPointer] = programCounter;

                programCounter = nnn;

            break;
            // 3xkk
            case 0x3000:
                
                if(V[x] == kk)
                {
                
                    programCounter += 2;
                
                }

            break;
            // 4xkk
            case 0x4000:
                
                if(V[x] != kk)
                {
                    
                    programCounter += 2;
                
                }

            break;
            // 5xy0
            case 0x5000:
            
                if(V[x] == V[y]){

                    programCounter += 2;

                }

            break;
            // 6xkk
            case 0x6000:

                V[x] == kk;

            break;
            // 7xkk
            case 0x7000:
            
                V[x] += kk;

            break;
            
            case 0x8000 :
            
                switch (opcode & 0x000F)
                {
                    // 8xy0
                    case 0x0000:
                        V[x] = V[y];
                        break;
                    // 8xy1
                    case 0x0001:

                        V[x] = V[x] | V[y];

                        break;
                    // 8xy2
                    case 0x0002:

                        V[x] = V[x] & V[y];

                    break;
                    // 8xy3
                    case 0x0003:
                    // Set Vx = Vx XOR Vy.
                        V[x] = V[x] ^ V[y];

                    break;
                    // 8xy4
                    case 0x0004:

                        V[x] = V[x] + V[y];

                        if(V[x] + V[y] > 255)
                        {

                            V[0xF] = 1;

                        } else                
                        {

                            V[0xF] = 0;

                        }
                    break;
                    // 8xy5
                    case 0x0005:

                        if(V[x] > V[y])
                        {

                            V[0xF] = 1;

                        } 
                        else
                        {

                            V[0xF] = 0;

                        }

                        V[x] = V[x] - V[y];

                    break;
                    // 8xy6
                    case 0x0006:

                        V[0xF] = opcode & 0x1;
                        // divide by 2
                        V[x] = V[x] >> 1;

                    break;

                    case 0x0007:

                        if(V[y] > V[x])
                        {
                            V[0xF] = 1;
                        } 
                        else
                        {
                            V[0xF] = 0;
                        }

                        V[x] = V[y] - V[x];

                    break;

                    case 0x000E :
                    // If the most-significant bit (before shift) of Vx is 1, then VF is set to 1, otherwise to 0
                        V[0xF] = (V[x] >> 7);
                    // Vx is multiplied by 2
                        V[x] = V[x] << 1;
                    break;
                    default: 
                        std::cout << "unknown opcode" << opcode << std::endl;
                        exit(EXIT_FAILURE);
                        break;
                }
            // 9xy0
            case 0x9000:
                if(V[x] != V[y])
                {
                    
                    programCounter += 2;
                
                }
                break;
            //Annn
            case 0xA000:
                
                I = nnn;
                
                programCounter +=2;
                
                break;

            case 0xB000:

                programCounter = nnn + V[0x0];

                break;
            
            case 0xC000:

                V[x] = (rand() % 0x100) & kk;
                
                programCounter += 2;
            
                break;

            case 0xD000:
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

            // The interpreter reads n bytes from memory, starting at the address stored in I. 
            // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
            // Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
            // VF is set to 1, otherwise it is set to 0. 
            //If the sprite is positioned so part of it is outside the coordinates of the display, 
            //it wraps around to the opposite side of the screen. 
            //See instruction 8xy3 for more information on XOR, and section 2.4, Display,
            // for more information on the Chip-8 screen and sprites.

                break;
            // Ex9E
            case 0xE000 :
                switch (opcode & 0x00FF)
                {
                case 0x009E:
                // Skip next instruction if key with the value of Vx is pressed.
                // Checks the keyboard, and if the key corresponding to the value 
                //of Vx is currently in the down position, PC is increased by 2.
                    if()
                    {
                    
                        programCounter += 4;
                    
                    }

                    break;
                
                case 0x00A1:
                // Skip next instruction if key with the value of Vx is not pressed.
                // Checks the keyboard, and if the key corresponding to the value of
                // Vx is currently in the up position, PC is increased by 2.
                    if()
                    {
                        
                        programCounter += 4;
                    
                    }

                    break;

                default:
                    std::cout << "unknown opcode" << opcode << std::endl;
                    exit(EXIT_FAILURE);
                    break;
                }

            case 0xF000 :
                switch (opcode & 0xFF)
                {
                // Fx07
                case 0x7:
                // The value of DT is placed into Vx.
                    V[x] = delayTimer;
                    
                    break;
                // Fx0A
                case 0xA:
                    
                // Wait for a key press, store the value of the key in Vx.
                    
                    break;
                // Fx15
                case 0x15:
                // Set delay timer = Vx.
                    delayTimer = V[x];
                
                    break;
                // Fx18 
                case 0x18:
                // Set sound timer = Vx.   
                    soundTimer = V[x];
                    
                    break;
                // Fx1E
                case 0x1E:
                // Set I = I + Vx.

                    I = I + V[x];
                    
                    break; 
                // Fx29
                case 0x29:
                // The value of I is set to the location for the hexadecimal sprite corresponding 
                // to the value of Vx. See section 2.4, Display, for more information on the Chip-8 
                // hexadecimal font.
                    break;
                // Fx33
                case 0x33:
                // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                // The interpreter takes the decimal value of Vx, and places the hundreds 
                // digit in memory at location in I, the tens digit at location I+1, and 
                // the ones digit at location I+2.

                    break;
                // Fx55 
                case 0x55:
                // Store registers V0 through Vx in memory starting at location I.
                // The interpreter copies the values of registers V0 through Vx into
                //  memory, starting at the address in I.
                    
                    break;
                // Fx65 
                case 0x65:
                // Read registers V0 through Vx from memory starting at location I.
                // The interpreter reads values from memory starting at location I 
                // into registers V0 through Vx.
                    
                    break;                    

                default:
                    
                    std::cout << "unknown opcode" << opcode << std::endl;
                    
                    exit(EXIT_FAILURE);
                    
                    break;
                }
                break;

            default:
                
                std::cout << "unknown opcode" << opcode << std::endl;
                
                exit(EXIT_FAILURE);
                
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



// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
// n or nibble - A 4-bit value, the lowest 4 bits of the instruction
// x - A 4-bit value, the lower 4 bits of the high byte of the instruction
// y - A 4-bit value, the upper 4 bits of the low byte of the instruction
// kk or byte - An 8-bit value, the lowest 8 bits of the instruction



// 0nnn - SYS addr
// Jump to a machine code routine at nnn.

// This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.


// 00E0 - CLS
// Clear the display.


// 00EE - RET
// Return from a subroutine.

// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.


// 1nnn - JP addr
// Jump to location nnn.

// The interpreter sets the program counter to nnn.


// 2nnn - CALL addr
// Call subroutine at nnn.

// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.


// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk.

// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.


// 4xkk - SNE Vx, byte
// Skip next instruction if Vx != kk.

// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.


// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy.

// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.


// 6xkk - LD Vx, byte
// Set Vx = kk.

// The interpreter puts the value kk into register Vx.


// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.

// Adds the value kk to the value of register Vx, then stores the result in Vx.

// 8xy0 - LD Vx, Vy
// Set Vx = Vy.

// Stores the value of register Vy in register Vx.


// 8xy1 - OR Vx, Vy
// Set Vx = Vx OR Vy.

// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.


// 8xy2 - AND Vx, Vy
// Set Vx = Vx AND Vy.
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

// The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.


// 8xy3 - XOR Vx, Vy
// Set Vx = Vx XOR Vy.

// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.


// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.

// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.


// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.

// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.


// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.

// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.


// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.

// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.


// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1.

// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.


// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.

// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.


// Annn - LD I, addr
// Set I = nnn.

// The value of register I is set to nnn.


// Bnnn - JP V0, addr
// Jump to location nnn + V0.

// The program counter is set to nnn plus the value of V0.


// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.

// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.


// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

// The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.


// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.

// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.


// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.

// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.


// Fx07 - LD Vx, DT
// Set Vx = delay timer value.

// The value of DT is placed into Vx.


// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.

// All execution stops until a key is pressed, then the value of that key is stored in Vx.


// Fx15 - LD DT, Vx
// Set delay timer = Vx.

// DT is set equal to the value of Vx.


// Fx18 - LD ST, Vx
// Set sound timer = Vx.

// ST is set equal to the value of Vx.


// Fx1E - ADD I, Vx
// Set I = I + Vx.

// The values of I and Vx are added, and the results are stored in I.


// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.

// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.


// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I+1, and I+2.

// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.


// Fx55 - LD [I], Vx
// Store registers V0 through Vx in memory starting at location I.

// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.


// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.

// The interpreter reads values from memory starting at location I into registers V0 through Vx.
