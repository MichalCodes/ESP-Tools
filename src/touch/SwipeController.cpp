#include "SwipeControler.h"

SwipeController::SwipeController(LGFX &tft) : tft(tft), touchStartX(-1), touchStartY(-1), lastSwipe(NONE) {}

void SwipeController::update() {
    int touchX, touchY;
    bool touching = tft.getTouch(&touchX, &touchY);

    if(touching) {
        if(touchStartX == -1) {
            touchStartX = touchX;
            touchStartY = touchY;
        }
    } else if(touchStartX != -1) {
        int dx = touchEndX - touchStartX;
        int dy = touchEndY - touchStartY;

        dx = (touchEndX == -1) ? 0 : dx;
        dy = (touchEndY == -1) ? 0 : dy;

        if(abs(dx) > abs(dy)) {
            if(dx > 20) lastSwipe = RIGHT;
            else if(dx < -20) lastSwipe = LEFT;
        } else {
            if(dy > 20) lastSwipe = DOWN;
            else if(dy < -20) lastSwipe = UP;
        }

        touchStartX = -1;
        touchStartY = -1;
        touchEndX = -1;
        touchEndY = -1;
    }

    if(touching) {
        touchEndX = touchX;
        touchEndY = touchY;
    }
}

SwipeController::Swipe SwipeController::getSwipe() {
    Swipe s = lastSwipe;
    lastSwipe = NONE; 
    return s;
}