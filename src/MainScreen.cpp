#include "MainScreen.h"
#include <Arduino.h>

MainScreen::MainScreen(LGFX& display, 
           const char* title,
           SettingsManager& settings)
    : tft(display),
      settingsManager(settings),
      screenTitle(title),
      settingsButton(700, 425, 95, 50, "Setting", tft, TFT_RED, TFT_WHITE)
{}

void MainScreen::addModule(IAppModule& module) {
    modules.push_back(&module);
}

void MainScreen::draw() {
    tft.fillScreen(TFT_BLUE);
    tft.fillRect(0, 0, 800, 70, TFT_GREEN);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(50, 30);
    tft.println(screenTitle);

    settingsButton.draw();
    
    int startX = 50;
    int startY = 100;
    int currentX = startX;
    int currentY = startY;
    
    iconAreas.clear();

    for (IAppModule* module : modules) {
        if (module == nullptr) continue; 

        // 1. Vykreslení oblasti tlačítka
        tft.fillRect(currentX, currentY, ICON_WIDTH, ICON_HEIGHT, TFT_WHITE);
        tft.drawRect(currentX, currentY, ICON_WIDTH, ICON_HEIGHT, TFT_BLACK);
        
        // 2. Vykreslení textu 
        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(2); 
        tft.setTextDatum(MC_DATUM); 
        tft.drawString(module->getName(), currentX + ICON_WIDTH/2, currentY + ICON_HEIGHT/2);

        // 3. Uložení dotykové oblasti
        iconAreas.push_back({currentX, currentY, ICON_WIDTH, ICON_HEIGHT, module});

        currentX += ICON_WIDTH + ICON_SPACING;
        
        if (currentX > 800 - ICON_WIDTH) { 
            currentX = startX;
            currentY += ICON_HEIGHT + ICON_SPACING;
        }
    }
}

bool MainScreen::isSettingsButtonTouched() {
    return settingsButton.isTouched();
}

IAppModule* MainScreen::handleTouch() {
    int touchX, touchY;
    
    if (tft.getTouch(&touchX, &touchY)) {
        
        // 1. Tlačítko Nastavení
        if (isSettingsButtonTouched()) {
            settingsManager.show();
            return settingsManager.isActive() ? nullptr : nullptr; 
        }
        
        // 2. Dynamické ikony
        for (const auto& area : iconAreas) {
            if (touchX >= area.x && touchX <= area.x + area.w &&
                touchY >= area.y && touchY <= area.y + area.h) {
                
                area.module->run();
                return area.module; 
            }
        }
    }
    return nullptr;
}