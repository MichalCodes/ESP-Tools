#pragma once

#include "gfx_conf.h" // Pro LGFX a definice barev (TFT_YELLOW, TFT_BLACK, atd.)
#include <Arduino.h>  // Pro třídu String

class Button {
public:
    Button(int x, int y, int w, int h, const char* label, LGFX &tftRef,
           uint16_t bgColor = TFT_YELLOW, uint16_t textColor = TFT_BLACK, uint8_t textSize = 2)
        : x(x), y(y), w(w), h(h), 
          // Ukládáme text do Stringu, abychom jej mohli měnit pomocí setText()
          currentLabel(label), 
          tft(tftRef),
          bgColor(bgColor), textColor(textColor), textSize(textSize) {}

    // Původní draw() je nyní volána, když se volá draw() bez argumentů,
    // a používá uloženou barvu pozadí.
    void draw() {
        draw(bgColor); // Použij uloženou barvu pozadí
    }

    /**
     * @brief Vykreslí tlačítko, volitelně s novou barvou pozadí.
     * @param newBgColor Nová barva pozadí. Pokud je -1, použije uloženou barvu.
     */
    void draw(int newBgColor) {
        if (newBgColor != -1) {
            bgColor = newBgColor; // Uloží novou barvu
        }
        
        tft.fillRect(x, y, w, h, bgColor);
        tft.setTextColor(textColor);
        tft.setTextSize(textSize);
        
        // POUŽITÍ: tft.setTextDatum(MC_DATUM); a tft.drawString() by zajistilo lepší centrování.
        // Zde zachovávám váš styl setCursor + println:
        tft.setCursor(x + 10, y + (h / 2) - 8); 
        tft.println(currentLabel); 
        
        // Volitelně přidejte rámeček: tft.drawRect(x, y, w, h, TFT_WHITE);
    }

    bool isTouched() {
        int touchX, touchY;
        // POZNÁMKA: Většina displejů (včetně LGFX/TFT_eSPI) vyžaduje kalibraci dotyku.
        // Předpokládáme, že tft.getTouch vrací kalibrované souřadnice.
        if (tft.getTouch(&touchX, &touchY)) {
            if (touchX >= x && touchX <= x + w && touchY >= y && touchY <= y + h) return true;
        }
        return false;
    }

    // NOVÁ METODA: Získá aktuální text
    String getText() const {
        return currentLabel;
    }

    // NOVÁ METODA: Nastaví nový text (label)
    void setText(const char* newLabel) {
        currentLabel = newLabel;
    }

private:
    int x, y, w, h;
    // ZMĚNA: Používáme String pro ukládání labelu, aby se dal měnit.
    String currentLabel; 
    LGFX &tft;
    uint16_t bgColor;
    uint16_t textColor;
    uint8_t textSize;
};
