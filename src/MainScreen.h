#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP

#include <vector>
#include "gfx_conf.h" 
#include "Button.h"
#include "Settings.h" 
#include "IAppModule.h" 

#define ICON_WIDTH 100
#define ICON_HEIGHT 80
#define ICON_SPACING 20

class MainScreen {
private:
    LGFX& tft;
    SettingsManager& settingsManager;
    const char* screenTitle;
    
    Button settingsButton;
    std::vector<IAppModule*> modules;
    
    struct IconArea {
        int x, y, w, h;
        IAppModule* module;
    };
    std::vector<IconArea> iconAreas;


public:
    MainScreen(LGFX& display, 
               const char* title,
               SettingsManager& settings);

    void addModule(IAppModule& module);

    void draw();
    
    bool isSettingsButtonTouched();

    IAppModule* handleTouch();
};

#endif