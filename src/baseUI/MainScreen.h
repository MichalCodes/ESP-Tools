#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP

#include <vector>
#include "hw/gfx_conf.h" 
#include "touch/Button.h"
#include "Settings.h" 
#include "IAppModule.h" 

#define ICON_WIDTH 100
#define ICON_HEIGHT 80
#define ICON_SPACING 20

/**
 * @class MainScreen
 * @brief Správce hlavní obrazovky (Launcheru), který vykresluje ikony aplikací a menu.
 * * @details Třída dynamicky spravuje registrované moduly a umožňuje jejich spouštění.
 * Příklad inicializace:
 * @code
 * MainScreen home(tft, "Moje ESP32", settings);
 * home.addModule(weatherApp);
 * home.draw();
 * @endcode
 */
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
    /**
     * @brief Konstruktor hlavní obrazovky.
     * @param display Reference na LovyanGFX displej pro vykreslování.
     * @param title Titulek zobrazený v horní části obrazovky.
     * @param settings Reference na SettingsManager pro ovládání nastavení.
     */
    MainScreen(LGFX& display, 
               const char* title,
               SettingsManager& settings);

    /**
     * @brief Zaregistruje nový modul (aplikaci) do hlavního menu.
     * @param module Reference na objekt implementující rozhraní IAppModule.
     */
    void addModule(IAppModule& module);

    /**
     * @brief Vykreslí kompletní GUI hlavní obrazovky včetně ikon a pozadí.
     * @note Tato metoda přepočítává pozice ikon v iconAreas na základě počtu modulů.
     */
    void draw();
    
    /**
     * @brief Zkontroluje, zda bylo stisknuto tlačítko pro vstup do nastavení.
     * @return true pokud detekuje stisk tlačítka Settings.
     */
    bool isSettingsButtonTouched();

    /**
     * @brief Zpracuje dotyk na ploše a identifikuje vybraný modul.
     * @details Prochází seznam 'iconAreas' a porovnává souřadnice dotyku s ikonami.
     * @return Ukazatel na vybraný IAppModule, nebo nullptr, pokud nebylo nic vybráno.
     */
    IAppModule* handleTouch();
};

#endif