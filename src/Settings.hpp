#pragma once

#include "gfx_conf.h" 
#include "Button.hpp" 
#include "AudioManager.h" 
#include "TrackBar.hpp"

// Důležité: Předpokládá se, že tft je definováno globálně (extern LGFX tft;)

/**
 * @brief Třída pro správu nastavení hry (hlasitosti) vrátane grafického ovládania.
 */
class SettingsManager {
private:
    AudioManager& musicManager;
    
    bool active = false; 
    uint8_t defaultVolume; // Uloží poslední hlasitost > 0 (pro Unmute)

    // Posuvník hlasitosti
    TrackBar volumeTrackBar;

    // Tlačítka pro rychlé ovládání
    Button muteButton;
    // Button sfxButton; // FIX: TOTO BYLO ODSTRANĚNO
    Button backButton; // NOVÝ PRVEK: Tlačítko pro návrat

public:
    /**
     * @brief Konstruktor SettingsManageru.
     */
    SettingsManager(AudioManager& audioManager) : musicManager(audioManager),
          volumeTrackBar(tft, 150, 200, 500, 40, 0, 21, musicManager.getVolume()),
          muteButton(150, 350, 200, 80, "MUTE / ON", tft, TFT_ORANGE, TFT_BLACK),
          backButton(650, 300, 100, 50, "ZPET", tft, TFT_BLUE, TFT_WHITE)
    {
        // Uložíme startovní hlasitost, pokud je > 0
        defaultVolume = musicManager.getVolume() > 0 ? musicManager.getVolume() : 10;
    }

    // --- Stavové metody ---
    
    bool isActive() const { return active; }
    
    void show() {
        active = true;
        drawScreen();
    }

    void hide() {
        active = false;
    }

    // --- Vykreslovací metody ---

    /**
     * @brief Vykreslí celou obrazovku nastavení.
     */
    void drawScreen() {
        tft.fillScreen(TFT_DARKGREY);
        tft.fillRect(0, 0, 800, 70, TFT_BLUE);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(50, 30);
        tft.println("Nastaveni zvuku");

        tft.setTextSize(2);
        tft.setCursor(50, 180);
        tft.println("Hlasitost:");

        // Vykreslení prvků
        volumeTrackBar.draw();
        muteButton.draw();
        backButton.draw(); // Vykreslení tlačítka Zpět
        
        drawVolumeValue(volumeTrackBar.getValue());
    }

    /**
     * @brief Vykreslí aktuální číselnou úroveň hlasitosti vedle posuvníku.
     */
    void drawVolumeValue(int volume) {
        // Smazání staré hodnoty
        tft.fillRect(660, 180, 80, 40, TFT_DARKGREY); 
        tft.setTextColor(TFT_YELLOW);
        tft.setTextSize(3);
        tft.setTextDatum(TL_DATUM);
        tft.setCursor(660, 190);
        tft.print(volume); 
    }

    // --- Zpracování vstupu ---

    /**
     * @brief Zpracuje vstup (dotyk) na obrazovce nastavení.
     * @return true, pokud zůstáváme v Settings; false, pokud odcházíme (stisk ZPET).
     */
    bool handleTouch() {
        int touchX, touchY;
        if (tft.getTouch(&touchX, &touchY)) {
            
            // 1. Zpracování posuvníku (TrackBar)
            if (volumeTrackBar.handleTouch(touchX, touchY)) {
                int newVolume = volumeTrackBar.getValue();
                musicManager.setVolume(newVolume);
                drawVolumeValue(newVolume);
                
                // Aktualizace defaultVolume (pouze pokud je > 0)
                if (newVolume > 0) {
                    defaultVolume = newVolume;
                }
                return true;
            }

            // 2. Tlačítko Mute/On
            if (muteButton.isTouched()) {
                if (musicManager.getVolume() > 0) {
                    // Ztlumit (Mute)
                    musicManager.setVolume(0);
                    volumeTrackBar.setValue(0);
                    drawVolumeValue(0);
                } else {
                    // Obnovit (Unmute)
                    uint8_t vol = defaultVolume > 0 ? defaultVolume : 10;
                    musicManager.setVolume(vol);
                    volumeTrackBar.setValue(vol);
                    drawVolumeValue(vol);
                }
                return true;
            }
            
            // 3. Tlačítko Zpět
            if (backButton.isTouched()) {
                hide();
                // Návrat 'false' signalizuje hlavní smyčce, že obrazovka nastavení skončila
                return false; 
            }
        }
        // Pokud nedošlo k žádné interakci, zůstáváme v nastavení
        return true; 
    }
};