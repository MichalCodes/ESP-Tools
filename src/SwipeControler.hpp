#pragma once
class SwipeController {
public:
    SwipeController(LGFX &tft) : tft(tft), touchStartX(-1), touchStartY(-1), lastSwipe(NONE) {}

    void update() {
        int touchX, touchY;
        bool touching = tft.getTouch(&touchX, &touchY);

        if(touching) {
            if(touchStartX == -1) {
                // začátek dotyku
                touchStartX = touchX;
                touchStartY = touchY;
            }
            // během dotyku nic neděláme
        } else if(touchStartX != -1) {
            // konec dotyku, spočítáme směr swipu
            int dx = touchEndX - touchStartX;
            int dy = touchEndY - touchStartY;

            // pokud touchEnd nebylo aktualizováno, použij touchStart (minimal swipe = 0)
            dx = (touchEndX == -1) ? 0 : dx;
            dy = (touchEndY == -1) ? 0 : dy;

            if(abs(dx) > abs(dy)) {
                if(dx > 20) lastSwipe = RIGHT;
                else if(dx < -20) lastSwipe = LEFT;
            } else {
                if(dy > 20) lastSwipe = DOWN;
                else if(dy < -20) lastSwipe = UP;
            }

            // reset
            touchStartX = -1;
            touchStartY = -1;
            touchEndX = -1;
            touchEndY = -1;
        }

        // ukládání poslední pozice při dotyku
        if(touching) {
            touchEndX = touchX;
            touchEndY = touchY;
        }
    }

    enum Swipe { NONE, UP, DOWN, LEFT, RIGHT };

    Swipe getSwipe() {
        Swipe s = lastSwipe;
        lastSwipe = NONE;  // po přečtení se smaže
        return s;
    }

private:
    LGFX &tft;
    int touchStartX, touchStartY;
    int touchEndX = -1, touchEndY = -1;
    Swipe lastSwipe;
};
