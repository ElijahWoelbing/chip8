#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Chip8.cpp";


bool init();

bool createWindow();

bool createRenderer();

const int screenWidth = 1024;

const int screenHeight = 512;

Chip8 chip8 = Chip8();

unsigned char keys[16] = {
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_r,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_f,
    SDLK_z,
    SDLK_x,
    SDLK_c,
    SDLK_v,
};

SDL_Window *window;

SDL_Renderer *renderer;


int main(int argc, char* argv[])
{
    bool quit = false;

    if(init())
    {

        if(createWindow() && createRenderer())
        {    
            chip8.initialize();
            
            chip8.loadProgram("../roms/Tetris.ch8");

            SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                64, 32);

            unsigned int pixels[2048];

            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e))
                {
                    if(e.type == SDL_QUIT)
                    {
                        
                        quit = true;

                    } else if(e.type == SDL_KEYDOWN)
                    {
                        for(int i = 0; i < 16; i++)
                        {
                            if(keys[i] == e.key.keysym.sym)
                            {
                                chip8.keys[i] = 1;
                            }
                        }

                    } else if(e.type == SDL_KEYUP)
                    {
                        for(int i = 0; i < 16; i++)
                        {
                            if(keys[i] == e.key.keysym.sym)
                            {
                                chip8.keys[i] = 0;
                            }
                        }
                    }
                }

                chip8.emulate();

                if(chip8.draw)
                {
                    chip8.draw = false;

                    for (int i = 0; i < 2048; i++)
                    {
                        unsigned int pixel = chip8.screen[i];
                        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
                    }

                    // Update SDL texture
                    SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
                    // Clear screen and render
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                        
                }
                
                std::this_thread::sleep_for(std::chrono::microseconds(1200));
            }
        }
        
    }

    SDL_DestroyWindow(window);

    return 0;

}

bool init()
{

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
    
        std::cout<< "Unable to initialize SDL Error:" << SDL_GetError() << std::endl;
        
        return false;
    
    }

    return true;

}

bool createWindow()
{
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        
    if(window == NULL)
    {
        printf("unable to create window Error: %s\n", SDL_GetError());

        return false;
    }

    return true;
}

bool createRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    if(renderer == NULL)
    {
        printf("unable to create renderer Error: %s\n", SDL_GetError());

        return false;
    }
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
    return true;
}

