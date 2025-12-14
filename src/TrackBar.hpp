
// Váš hlavičkový soubor gfx_conf.h musí definovat typ LGFX a konstanty barev (TFT_RED atd.)
#pragma once
#include "gfx_conf.h" 


/**
 * @brief Třída implementující posuvník (Slider/TrackBar) pro dotykovou obrazovku.
 */
class TrackBar {
private:
    // Měníme typ reference z TFT_eSPI na LGFX (nebo jiný typ, který používáte)
    // Pokud je LGFX definováno v gfx_conf.h, použijeme jej.
    LGFX& tft;
    
    // Rozměry a pozice celého posuvníku
    int x;
    int y;
    int width;
    int height;
    
    // Rozsah hodnot
    int minValue;
    int maxValue;
    int currentValue;
    
    // Vizuální prvky
    int lineY; // Y pozice osy posuvníku
    int handleRadius = 15; // Poloměr ovládacího bodu (handle)

public:
    /**
     * @brief Konstruktor TrackBaru.
     * @param tftObj Reference na LGFX objekt.
     * @param startX Počáteční X pozice.
     * @param startY Počáteční Y pozice.
     * @param w Šířka posuvníku.
     * @param h Výška (používá se pro detekci dotyku).
     * @param minVal Minimální hodnota.
     * @param maxVal Maximální hodnota.
     * @param initialVal Počáteční hodnota.
     */
    TrackBar(LGFX& tftObj, int startX, int startY, int w, int h, 
             int minVal, int maxVal, int initialVal)
        : tft(tftObj), x(startX), y(startY), width(w), height(h),
          minValue(minVal), maxValue(maxVal), currentValue(initialVal) 
    {
        lineY = y + height / 2;
    }

    /**
     * @brief Vykreslí celý posuvník.
     */
    void draw() {
        tft.drawFastHLine(x, lineY, width, TFT_WHITE);
        drawHandle();
    }
    
    /**
     * @brief Vykreslí nebo překreslí rukojeť posuvníku.
     */
    void drawHandle() {
        // Smaž starou rukojeť
        tft.fillRect(x - handleRadius, lineY - handleRadius, 
                     width + 2 * handleRadius, 2 * handleRadius, 
                     TFT_DARKGREY);
        
        // Překreslení osy
        tft.drawFastHLine(x, lineY, width, TFT_WHITE);

        // Výpočet X pozice rukojeti
        float ratio = (float)(currentValue - minValue) / (maxValue - minValue);
        int handleX = x + (int)(ratio * width);

        // Vykreslení nové rukojeti
        tft.fillCircle(handleX, lineY, handleRadius, TFT_RED);
    }
    
    /**
     * @brief Zpracuje dotyk a aktualizuje hodnotu, pokud dotyk spadá do oblasti.
     */
    bool handleTouch(int touchX, int touchY) {
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

    /**
     * @brief Nastaví novou hodnotu.
     */
    void setValue(int val) {
        if (val < minValue) val = minValue;
        if (val > maxValue) val = maxValue;
        currentValue = val;
        drawHandle();
    }
    
    /**
     * @brief Získá aktuální hodnotu.
     */
    int getValue() const {
        return currentValue;
    }
};


