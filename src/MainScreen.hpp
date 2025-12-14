// MainScreen.hpp

#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP

#include <vector>
#include "gfx_conf.h" 
#include "Button.hpp"
#include "Settings.hpp" 
#include "IAppModule.h" 

// Rozměry ikon a rozestupy
#define ICON_WIDTH 100
#define ICON_HEIGHT 80
#define ICON_SPACING 20

class MainScreen {
private:
    LGFX& tft;
    SettingsManager& settingsManager;
    const char* screenTitle;
    
    Button settingsButton;

    // FIX: Změněno z IGameModule* na IAppModule*
    std::vector<IAppModule*> modules;
    
    struct IconArea {
        int x, y, w, h;
        // FIX: Změněno z IGameModule* na IAppModule*
        IAppModule* module;
    };
    std::vector<IconArea> iconAreas;


public:
    MainScreen(LGFX& display, 
               const char* title,
               SettingsManager& settings)
        : tft(display),
          settingsManager(settings),
          screenTitle(title),
          settingsButton(700, 425, 95, 50, "Setting", tft, TFT_RED, TFT_WHITE)
    {}

    /**
     * @brief Přidá herní modul, který se zobrazí jako ikona na obrazovce.
     */
    // FIX: Změněno z IGameModule& na IAppModule&
    void addModule(IAppModule& module) {
        modules.push_back(&module);
    }


    /**
     * @brief Vykreslí celou hlavní obrazovku s titulkem a všemi ikonami.
     */
    void draw() {
        tft.fillScreen(TFT_BLUE);
        tft.fillRect(0, 0, 800, 70, TFT_GREEN);
        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(3);
        tft.setCursor(20, 31);
        tft.println(screenTitle);

        settingsButton.draw(); 
        
        iconAreas.clear();

        int startX = 20; 
        int currentX = startX;
        int currentY = 100;

        for (IAppModule* module : modules) {
            
            // 1. Vykreslení oblasti tlačítka
            tft.fillRect(currentX, currentY, ICON_WIDTH, ICON_HEIGHT, TFT_WHITE);
            tft.drawRect(currentX, currentY, ICON_WIDTH, ICON_HEIGHT, TFT_BLACK);
            
            // 2. Vykreslení textu (jako zástupce ikony)
            tft.setTextColor(TFT_BLACK);
            tft.setTextSize(2); 
            tft.setTextDatum(MC_DATUM); 
            tft.drawString(module->getName(), currentX + ICON_WIDTH/2, currentY + ICON_HEIGHT/2);

            // 3. Uložení dotykové oblasti
            iconAreas.push_back({currentX, currentY, ICON_WIDTH, ICON_HEIGHT, module});

            currentX += ICON_WIDTH + ICON_SPACING;
        }
    }
    
    bool isSettingsButtonTouched() {
        return settingsButton.isTouched();
    }

    bool handleTouch() {
        int touchX, touchY;
        
        if (tft.getTouch(&touchX, &touchY)) {
            
            // 1. Tlačítko Nastavení
            if (isSettingsButtonTouched()) {
                settingsManager.show();
                return true; 
            }
            
            // 2. Dynamické ikony
            for (const auto& area : iconAreas) {
                if (touchX >= area.x && touchX <= area.x + area.w &&
                    touchY >= area.y && touchY <= area.y + area.h) {
                    
                    area.module->run(); 
                    return true; 
                }
            }
        }
        
        return false; 
    }
};

#endif // MAIN_SCREEN_HPP