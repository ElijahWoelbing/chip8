#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include "Chip8.cpp";


bool init();

bool createWindow();

bool createRenderer();

const int screenWidth = 640;

const int screenHeight = 320;

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
            
            chip8.loadProgram("./roms/tetris.ch8");

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
                    SDL_Rect pixle;
                    for(int i = 0; i < 32; i++)
                    {
                        for (int j = 0; j < 64; j++)
                        {
                            
                            pixle.x = j;
                            
                            pixle.y = i;
                            
                            pixle.h = 10;

                            pixle.w = 10;
                            
                            if( chip8.screen[i + j] = 1)
                            {
                                SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );        
                            } else
                            {
                                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );                                    
                            }
                       
                            SDL_RenderFillRect( renderer, &pixle);
                       
                        }

                    }
                        
                }
                
                
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
    window = SDL_CreateWindow("chip", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        
    if(window == NULL)
    {
        printf("unable to create window Error: %s\n", SDL_GetError());

        return false;
    }

    return true;
}

bool createRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if(renderer == NULL)
    {
        printf("unable to create renderer Error: %s\n", SDL_GetError());

        return false;
    }

    return true;
}