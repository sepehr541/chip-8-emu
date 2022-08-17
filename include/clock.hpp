#ifndef CLOCK_H
#define CLOCK_H

///
/// Clock structure that will tick at given frequency
///
struct Clock
{
    bool ticked = false;

    // in ms
    float delay;

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    Clock(){}

    Clock(int frequency)
    {
        delay = 1000.0 / frequency;
    }

    void tick()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - start).count();

        if (dt >= delay)
        {
            ticked = true;
            start = currentTime;
        }
        else
        {
            ticked = false;
        }
    }
};


#endif
