#include "TrackBar.h"

TrackBar::TrackBar(LGFX& tftObj, int startX, int startY, int w, int h, 
         int minVal, int maxVal, int initialVal)
    : tft(tftObj), x(startX), y(startY), width(w), height(h),
      minValue(minVal), maxValue(maxVal), currentValue(initialVal)
{
    lineY = y + h / 2;
    if (currentValue < minValue) currentValue = minValue;
    if (currentValue > maxValue) currentValue = maxValue;
}

void TrackBar::draw() {
    tft.fillRect(x - handleRadius, y, 
                 width + 2 * handleRadius, height, 
                 TFT_DARKGREY); 

    tft.drawFastHLine(x, lineY, width, TFT_WHITE);

    drawHandle();
}

void TrackBar::drawHandle() {
    tft.fillRect(x - handleRadius, lineY - handleRadius, 
                 width + 2 * handleRadius, 2 * handleRadius, 
                 TFT_DARKGREY);
    
    tft.drawFastHLine(x, lineY, width, TFT_WHITE);

    float ratio = (float)(currentValue - minValue) / (maxValue - minValue);
    int handleX = x + (int)(ratio * width);

    tft.fillCircle(handleX, lineY, handleRadius, TFT_RED);
}

bool TrackBar::handleTouch(int touchX, int touchY) {
    if (touchX >= x && touchX <= x + width && 
        touchY >= y && touchY <= y + height) 
    {
        int newX = touchX - x;
        
        if (newX < 0) newX = 0;
        if (newX > width) newX = width;

        int newValue = (newX * (maxValue - minValue)) / width + minValue;
        
        if (newValue != currentValue) {
            currentValue = newValue;
            drawHandle();
            return true;
        }
    }
    return false;
}

int TrackBar::getValue() const {
    return currentValue;
}

void TrackBar::setValue(int val) {
    if (val < minValue) val = minValue;
    if (val > maxValue) val = maxValue;
    
    if (val != currentValue) {
        currentValue = val;
        drawHandle();
    }
}