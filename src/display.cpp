#include <SDL.h>
#include <iostream>
#include "display.h"
#include "chip8.h"

Display::Display(int w, int h, int s)
{
    width = w;
    height = h;
    scale = s;

    window = SDL_CreateWindow(
        "Emu",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width * scale, height * scale, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetScale(renderer, scale, scale);
    SDL_RenderSetLogicalSize(renderer, width, height);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC,
        width, height);
}

Display::~Display()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

int Display::mapKeyToIndex(SDL_Keycode key)
{
    if (key >= SDLK_0 && key <= SDLK_9)
        return key - SDLK_0;
    else if (key >= SDLK_a && key <= SDLK_f)
        return key - SDLK_a + 10;
    else
        return -1;
}

void Display::loop(Chip8 *chip8)
{
    bool quit = false;
    SDL_Event event;
    Uint32 *pixels = new Uint32[width * height];
    // handle events
    while (!quit)
    {
        SDL_WaitEventTimeout(&event, 0);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            chip8->setKey(mapKeyToIndex(event.key.keysym.sym), event.type == SDL_KEYDOWN);
            break;
        }
        chip8->emulateCycle();
        if (chip8->getRefreshFlag())
        {
            chip8->getGfx(pixels);
            SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}
