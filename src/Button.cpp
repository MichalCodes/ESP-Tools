#include "Button.h"

Button::Button(int x, int y, int w, int h, const char* label, LGFX &tftRef,
           uint16_t bgColor, uint16_t textColor, uint8_t textSize)
    : x(x), y(y), w(w), h(h), 
      currentLabel(label), 
      tft(tftRef),
      bgColor(bgColor), textColor(textColor), textSize(textSize) {}

void Button::draw() {
    draw(bgColor); 
}

void Button::draw(int newBgColor) {
    if (newBgColor != -1) {
        bgColor = newBgColor; 
    }
    
    tft.fillRect(x, y, w, h, bgColor);
    tft.setTextColor(textColor);
    tft.setTextSize(textSize);
    
    tft.setCursor(x + 10, y + (h / 2) - 8); 
    tft.println(currentLabel); 
}

bool Button::isTouched() {
    int touchX, touchY;
    if (tft.getTouch(&touchX, &touchY)) {
        if (touchX >= x && touchX <= x + w && touchY >= y && touchY <= y + h) return true;
    }
    return false;
}

void Button::setText(const char* label) {
    currentLabel = label;
    draw(); 
}