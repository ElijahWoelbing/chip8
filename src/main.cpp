#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "Chip8.cpp"
void init();
void createWindow();
void createRenderer();
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
    init();
    createWindow();
    createRenderer();
    chip8.initialize();
    std::string romName;
    bool romLoaded = false;
    while (!romLoaded)
    {
        std::cout << "please enter path to rom" << std::endl;
        std::cin >> romName;
        if(chip8.loadProgram(romName.data()))
        {
            romLoaded = true;
        }
    }
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    unsigned int pixels[2048];
    SDL_Event e;
    while (!quit)
    {
        chip8.emulate();
        while (SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            } 
           else if(e.type == SDL_KEYDOWN)
            {
                for(int i = 0; i < 16; i++)
                {
                    if(keys[i] == e.key.keysym.sym)
                    {
                        chip8.keys[i] = 1;
                    }
                }
            } 
            else if(e.type == SDL_KEYUP)
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
        if(chip8.draw)
        {
            chip8.draw = false;
            for (int i = 0; i < 2048; i++)
            {
                unsigned int pixel = chip8.screen[i];
                pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }
            SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    SDL_DestroyWindow(window);
    return 0;
}
void init() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout<< "Unable to initialize SDL Error:" << SDL_GetError() << std::endl;
        exit( 3 );
    }
}
void createWindow(){
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        std::cout << "unable to create window Error:" << SDL_GetError() << std::endl;
        exit( 3 );    
    }
}
void createRenderer() {
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL)
    {
        std::cout << "unable to create renderer Error:" << SDL_GetError() << std::endl;
        exit( 3 );
    }
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
}
