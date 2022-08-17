#include <SDL.h>
#include "display.hpp"
#include "chip8.hpp"

Display::Display(std::string title, int w, int h, int s)
{
    width = w;
    height = h;
    scale = s;

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width * scale, height * scale, 0);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    
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

void Display::draw(Uint32 *pixels)
{
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
