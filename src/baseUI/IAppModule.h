#ifndef IAPP_MODULE_H
#define IAPP_MODULE_H

#include "hw/gfx_conf.h"

/**
 * @class IAppModule
 * @brief Abstraktní rozhraní (Interface) pro moduly aplikací.
 * * @details Každá nová aplikace (např. Meteostanice, Přehrávač, Kalkulačka) musí 
 * dědit z této třídy a implementovat její virtuální metody. To umožňuje 
 * MainScreenu pracovat s různými aplikacemi jednotným způsobem.
 * * 
 */
class IAppModule {
public:
    /**
     * @brief Virtuální destruktor.
     * @details Zajišťuje správné uvolnění paměti u odvozených tříd.
     */
    virtual ~IAppModule() = default;

    /**
     * @brief Vrátí název aplikace.
     * @return Textový řetězec s názvem (např. "WiFi Scanner").
     */
    virtual const char* getName() const = 0;

    /**
     * @brief Vrátí cestu k souboru ikony na SD kartě.
     * @return Cesta k BMP nebo PNG souboru (např. "/icons/wifi.bmp").
     */
    virtual const char* getIconPath() const = 0; 

    /**
     * @brief Hlavní metoda pro spuštění vnitřní logiky aplikace.
     * @details Tato metoda je volána z MainScreenu při kliknutí na ikonu.
     */
    virtual void run() = 0;

    /**
     * @brief Zjistí, zda aplikace aktuálně běží.
     * @return true pokud je aplikace v popředí a aktivní.
     */
    virtual bool isActive() const = 0;
};

#endif