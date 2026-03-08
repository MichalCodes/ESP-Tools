#pragma once
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hw/gfx_conf.h"
#include <math.h>
#include <Arduino.h>

/**
 * @class Joystick
 * @brief Třída pro simulaci analogového joysticku na dotykovém displeji.
 * * @details Tento prvek umožňuje ovládat hry nebo aplikace pomocí virtuálního 
 * joysticku. Zajišťuje vykreslení základny (outer) a pohyblivého středu (inner), 
 * včetně logiky pro omezení pohybu v kruhu a automatického centrování po uvolnění.
 * * 
 */
class Joystick {
public:
    /**
     * @brief Konstruktor pro vytvoření virtuálního joysticku.
     * @param centerX X-ová souřadnice středu celého joysticku.
     * @param centerY Y-ová souřadnice středu celého joysticku.
     * @param outerRadius Poloměr vnějšího (pevného) kruhu.
     * @param innerRadius Poloměr vnitřního (pohyblivého) kruhu.
     * @param tft Reference na LovyanGFX displej pro vykreslování.
     * @param bgColor Barva pozadí pro překreslování pohybu (obvykle TFT_BLACK).
     */
    Joystick(int centerX, int centerY, int outerRadius, int innerRadius, LGFX &tft, uint16_t bgColor);

    /**
     * @brief Prvotní vykreslení celého joysticku (základna i střed).
     */
    void drawJoystick();
    
    /**
     * @brief Přesune střed joysticku na základě dotyku.
     * @details Metoda automaticky vypočítá vzdálenost od středu a zajistí, 
     * aby se vnitřní kruh nepohnul mimo hranice vnějšího kruhu.
     * @param touchX Aktuální X-ová souřadnice dotyku.
     * @param touchY Aktuální Y-ová souřadnice dotyku.
     */
    void moveJoystick(int touchX, int touchY);
    
    /**
     * @brief Zajišťuje plynulý návrat joysticku do středové polohy.
     * @details Tuto metodu je vhodné volat v hlavní smyčce hry, pokud 
     * 'isJoystickTouched' vrátí false.
     */
    void updateAutoCenter();
    
    /**
     * @brief Ověří, zda se zadaný dotyk nachází v oblasti joysticku.
     * @param touchX X-ová souřadnice dotyku.
     * @param touchY Y-ová souřadnice dotyku.
     * @return true pokud je dotyk v oblasti vnějšího kruhu.
     */
    bool isJoystickTouched(int touchX, int touchY);

    /**
     * @brief Okamžitě vycentruje joystick a překreslí jej do výchozí polohy.
     */
    void resetJoystick();

    /**
     * @brief Získá aktuální X-ovou souřadnici středu joysticku.
     * @return Hodnota X (lze mapovat na rychlost nebo směr pohybu).
     */
    int getJoystickX() const;

    /**
     * @brief Získá aktuální Y-ovou souřadnici středu joysticku.
     * @return Hodnota Y (lze mapovat na rychlost nebo směr pohybu).
     */
    int getJoystickY() const;

private:
    int centerX, centerY;     ///< Pevné souřadnice středu základny
    int outerRadius;          ///< Poloměr vnějšího kruhu
    int innerRadius;          ///< Poloměr vnitřního kruhu (jezdce)
    int joystickX, joystickY; ///< Aktuální proměnlivé souřadnice jezdce
    LGFX &tft;                ///< Reference na grafický ovladač
    uint16_t bgColor;         ///< Barva pozadí pro mazání stop pohybu
};

#endif