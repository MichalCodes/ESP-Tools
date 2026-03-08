#pragma once

#include "hw/gfx_conf.h"  

/**
 * @class Button
 * @brief Třída pro tvorbu a správu interaktivních grafických tlačítek.
 * * @details Umožňuje snadné vykreslování tlačítek s textem, změnu barev a 
 * detekci dotyku v definované oblasti. Ideální pro ovládací panely a menu.
 * * @code
 * Button btn(10, 10, 100, 50, "Start", tft);
 * btn.draw();
 * if (btn.isTouched()) {
 * // Akce při stisku
 * }
 * @endcode
 */
class Button {
public:
    /**
     * @brief Konstruktor tlačítka.
     * @param x X-ová souřadnice levého horního rohu.
     * @param y Y-ová souřadnice levého horního rohu.
     * @param w Šířka tlačítka v pixelech.
     * @param h Výška tlačítka v pixelech.
     * @param label Text zobrazený na tlačítku.
     * @param tftRef Reference na LovyanGFX objekt pro vykreslování.
     * @param bgColor Barva pozadí tlačítka (výchozí žlutá).
     * @param textColor Barva textu (výchozí černá).
     * @param textSize Velikost písma (výchozí 2).
     */
    Button(int x, int y, int w, int h, const char* label, LGFX &tftRef,
           uint16_t bgColor = TFT_YELLOW, uint16_t textColor = TFT_BLACK, uint8_t textSize = 2);

    /** @brief Vykreslí tlačítko na displej s aktuálně nastavenými parametry. */
    void draw();

    /** * @brief Vykreslí tlačítko s dočasnou změnou barvy (např. efekt stisku).
     * @param newBgColor Dočasná barva pozadí.
     */
    void draw(int newBgColor);

    /** * @brief Zkontroluje, zda je na tlačítko aktuálně klikáno.
     * @details Interně volá čtení dotyku z displeje a porovnává se souřadnicemi.
     * @return true pokud je detekován stisk v oblasti tlačítka.
     */
    bool isTouched();

    /** * @brief Změní text na tlačítku.
     * @note Pro zobrazení změny je nutné následně zavolat draw().
     * @param label Nový textový řetězec.
     */
    void setText(const char* label);

    /** * @brief Změní barvu pozadí tlačítka.
     * @param newBgColor Nová barva (např. TFT_RED, 0xF800).
     */
    void setColor(uint16_t newBgColor);

    /** * @brief Ruční kontrola, zda dané souřadnice leží uvnitř tlačítka.
     * @param x X souřadnice k ověření.
     * @param y Y souřadnice k ověření.
     * @return true pokud souřadnice [x,y] spadají do oblasti tlačítka.
     */
    bool checkCoordinates(int x, int y) const;

private:
    int x, y, w, h;
    String currentLabel;
    LGFX &tft;
    uint16_t bgColor;
    uint16_t textColor;
    uint8_t textSize;
};