#pragma once
#include "gfx_conf.h" 

class TrackBar {
private:
    LGFX& tft;
    
    int x;
    int y;
    int width;
    int height;
    
    int minValue;
    int maxValue;
    int currentValue;
    
    int lineY; 
    int handleRadius = 15; 

public:
    TrackBar(LGFX& tftObj, int startX, int startY, int w, int h, 
             int minVal, int maxVal, int initialVal);

    void draw();
    bool handleTouch(int touchX, int touchY);
    int getValue() const;
    void setValue(int val);

private:
    void drawHandle();
};