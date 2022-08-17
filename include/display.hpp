#include <SDL.h>
#include "chip8.hpp"
class Display
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width, height, scale;

public:
    Display(int width, int height, int scale);
    ~Display();
    void loop(Chip8& chip8);
    int mapKeyToIndex(SDL_Keycode key);
    bool handleEvents(Chip8 &chip8);
};