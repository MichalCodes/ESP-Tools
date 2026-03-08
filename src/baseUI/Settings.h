#pragma once

#include "hw/gfx_conf.h" 
#include "touch/Button.h" 
#include "hw/AudioManager.h" 
#include "touch/TrackBar.h"

/**
 * @class SettingsManager
 * @brief Správce obrazovky nastavení a uživatelského rozhraní pro audio.
 * * @details Tato třída vykresluje ovládací prvky (tlačítka, posuvníky) a propojuje 
 * uživatelský vstup přímo s instancí AudioManager.
 * * @code
 * SettingsManager settings(audioManager);
 * // V hlavní smyčce nebo při stisku tlačítka "Nastavení":
 * settings.show();
 * while(settings.isActive()) {
 * settings.update();
 * }
 * @endcode
 */
class SettingsManager {
private:
    AudioManager& musicManager;
    
    bool active = false; 
    uint8_t defaultVolume; 

    TrackBar volumeTrackBar;

    Button muteButton;
    Button backButton; 

public:
    /**
     * @brief Konstruktor správce nastavení.
     * @param audioManager Reference na existujícího správce audia pro synchronizaci hlasitosti.
     */
    SettingsManager(AudioManager& audioManager);

    /**
     * @brief Zjistí, zda je obrazovka nastavení aktuálně aktivní (viditelná).
     * @return true pokud je GUI nastavení zobrazeno.
     */
    bool isActive() const;

    /**
     * @brief Aktivuje režim nastavení a vykreslí celou obrazovku na displej.
     * @note Tato metoda změní stav vnitřní proměnné 'active' na true.
     */
    void show();

    /**
     * @brief Ukončí režim nastavení a skryje rozhraní.
     */
    void hide();

    /**
     * @brief Hlavní obslužná metoda pro UI prvky.
     * @details Musí být volána v cyklu, dokud je isActive() true. Zpracovává dotyky 
     * na posuvník hlasitosti a tlačítka.
     */
    void update(); 

private:
    /** @brief Vykreslí pozadí a statické prvky obrazovky. */
    void drawScreen();
    /** @brief Aktualizuje číselné zobrazení hlasitosti na displeji. */
    void drawVolumeValue(int volume);
    /** @brief Interní logika pro detekci kolizí dotyku s UI prvky. */
    bool handleTouch(int touchX, int touchY);
};