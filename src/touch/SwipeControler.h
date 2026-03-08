#pragma once
#include <LovyanGFX.hpp>
#include "hw/gfx_conf.h"

/**
 * @class SwipeController
 * @brief Ovladač pro detekci dotykových gest (přejetí prstem) na displeji.
 * * @details Sleduje trajektorii dotyku od stisknutí po uvolnění a vyhodnocuje 
 * směr pohybu (nahoru, dolů, doleva, doprava).
 * * 
 * * @code
 * SwipeController swipe(tft);
 * void loop() {
 * swipe.update();
 * if (swipe.getSwipe() == SwipeController::LEFT) {
 * // Přepnout na další stránku
 * }
 * }
 * @endcode
 */
class SwipeController {
public:
    /**
     * @enum Swipe
     * @brief Reprezentuje směr detekovaného gesta.
     */
    enum Swipe { 
        NONE,  ///< Žádné gesto nebylo detekováno
        UP,    ///< Přejetí prstem směrem nahoru
        DOWN,  ///< Přejetí prstem směrem dolů
        LEFT,  ///< Přejetí prstem směrem doleva
        RIGHT  ///< Přejetí prstem směrem doprava
    };
    
    /**
     * @brief Konstruktor ovladače gest.
     * @param tft Reference na instanci LovyanGFX pro přístup k dotykové vrstvě.
     */
    SwipeController(LGFX &tft);

    /**
     * @brief Periodická aktualizace stavu dotyku.
     * @details Tuto metodu je nutné volat v každém průchodu smyčkou loop(). 
     * Interně ukládá počáteční a koncové body dotyku pro výpočet gesta.
     */
    void update();

    /**
     * @brief Vrátí poslední detekované gesto.
     * @return Hodnota z enumerace Swipe. Po přečtení by se měl stav obvykle resetovat.
     */
    Swipe getSwipe();

private:
    LGFX &tft;
    int touchStartX, touchStartY;
    int touchEndX = -1, touchEndY = -1;
    Swipe lastSwipe;
};