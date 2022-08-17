#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <string>

#include "chip8.hpp"

class Display
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width, height, scale;

public:
    Display(std::string title, int width, int height, int scale);
    ~Display();
    void draw(Uint32* pixels);
};

#endif