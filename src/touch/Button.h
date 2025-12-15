#pragma once

#include "hw/gfx_conf.h"  

class Button {
public:
    Button(int x, int y, int w, int h, const char* label, LGFX &tftRef,
           uint16_t bgColor = TFT_YELLOW, uint16_t textColor = TFT_BLACK, uint8_t textSize = 2);

    void draw();
    void draw(int newBgColor);
    bool isTouched();
    void setText(const char* label);
    void setColor(uint16_t newBgColor);
    bool checkCoordinates(int x, int y) const;

private:
    int x, y, w, h;
    String currentLabel;
    LGFX &tft;
    uint16_t bgColor;
    uint16_t textColor;
    uint8_t textSize;
};