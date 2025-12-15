#pragma once
#include <LovyanGFX.hpp>
#include "hw/gfx_conf.h"

class SwipeController {
public:
    enum Swipe { NONE, UP, DOWN, LEFT, RIGHT };
    
    SwipeController(LGFX &tft);

    void update();
    Swipe getSwipe();

private:
    LGFX &tft;
    int touchStartX, touchStartY;
    int touchEndX = -1, touchEndY = -1;
    Swipe lastSwipe;
};