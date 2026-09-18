#pragma once
#include <stdint.h>

// Common interface for every content-area screen (Main, Height, Position).
class Screen
{
public:
    virtual ~Screen();

    // Prepares layout and default values before the first draw.
    virtual void init() = 0;

    // Paints this screen's content area.
    virtual void draw() = 0;

    // Routes an active touch to this screen.
    virtual void handleTouch(int16_t x, int16_t y) = 0;

    // Clears pressed and dragged state when the finger lifts.
    virtual void handleRelease() = 0;
};
