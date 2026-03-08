#pragma once
#include "hw/gfx_conf.h" 

/**
 * @class TrackBar
 * @brief Grafický ovládací prvek (posuvník) pro výběr číselné hodnoty v daném rozsahu.
 * * @details Umožňuje uživateli měnit hodnoty tažením nebo kliknutím na lištu.
 * 
 * * @code
 * TrackBar volumeBar(tft, 50, 100, 200, 40, 0, 100, 50);
 * volumeBar.draw();
 * * // V obsluze dotyku:
 * if (volumeBar.handleTouch(tx, ty)) {
 * int newVol = volumeBar.getValue();
 * }
 * @endcode
 */
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
    /**
     * @brief Konstruktor pro vytvoření posuvníku.
     * @param tftObj Reference na displej LovyanGFX.
     * @param startX X-ová souřadnice levého horního rohu.
     * @param startY Y-ová souřadnice levého horního rohu.
     * @param w Šířka celého prvku.
     * @param h Výška (ovlivňuje oblast citlivou na dotyk).
     * @param minVal Minimální možná hodnota (např. 0).
     * @param maxVal Maximální možná hodnota (např. 255).
     * @param initialVal Počáteční nastavená hodnota.
     */
    TrackBar(LGFX& tftObj, int startX, int startY, int w, int h, 
             int minVal, int maxVal, int initialVal);

    /**
     * @brief Vykreslí lištu a jezdec na displej.
     * @details Metoda vykresluje vodicí čáru a kruhový "handle" na pozici odpovídající currentValue.
     */
    void draw();

    /**
     * @brief Zpracuje dotykovou událost.
     * @param touchX X-ová souřadnice dotyku.
     * @param touchY Y-ová souřadnice dotyku.
     * @return true pokud byl dotyk uvnitř oblasti TrackBaru a hodnota se změnila.
     */
    bool handleTouch(int touchX, int touchY);

    /**
     * @brief Vrátí aktuálně nastavenou hodnotu.
     * @return Aktuální hodnota v rozsahu [minValue, maxValue].
     */
    int getValue() const;

    /**
     * @brief Programově nastaví novou hodnotu.
     * @param val Nová hodnota (bude automaticky omezena rozsahem min/max).
     */
    void setValue(int val);

private:
    /** @brief Interní metoda pro překreslení pouze jezdce (optimalizace vykreslování). */
    void drawHandle();
};