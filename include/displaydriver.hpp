#ifndef DISPLAYDRIVER_H
#define DISPLAYDRIVER_H

#include "chip8.hpp"

// Forward Declaration
class Display;

class DisplayDriver
{
    ///
    /// Mark - Members
    ///

private:
    Display &display;
    Uint32 pixels[Chip8::DIS_PIXELS];

    ///
    /// Mark - Constructors
    ///

public:
    DisplayDriver(Display &display): display(display){};


    ///
    /// Mark - Functions
    ///

public:
    void updateDisplay(std::vector<uint32_t>& vec)
    {
        for(int i = 0; i < vec.size(); i++)
            pixels[i] = vec[i];

        display.draw(this->pixels);
    }
    
};

#endif
