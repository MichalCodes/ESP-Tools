#ifndef TOOLS_H
#define TOOLS_H

/**
 * @file tools.h
 * @brief Hlavní rozcestník knihovny ESP-Tools.
 * * @details Tento soubor spravuje podmíněnou kompilaci všech modulů. 
 * Pro aktivaci konkrétního modulu definujte příslušné makro před inkluzí tohoto souboru.
 * * @section Příklad použití:
 * @code
 * #define USE_JOYSTICK
 * #define USE_AUDIOMANAGER
 * #include "tools.h"
 * * // Nyní jsou k dispozici třídy Joystick, FileLoader (automaticky) a AudioManager.
 * @endcode
 * * 
 */

#include "hw/gfx_conf.h"
#include <LovyanGFX.hpp>

// ============ TOUCH (Vstupní periferie) ============
#ifdef USE_JOYSTICK
    #include "touch/Joystick.h"
#endif

#ifdef USE_BUTTON
    #include "touch/Button.h"
#endif

#ifdef USE_TRACKBAR
    #include "touch/TrackBar.h"
#endif

#ifdef USE_SWIPECONTROLLER
    #include "touch/SwipeController.h"
#endif

// ============ HW (Hardware a soubory) ============
#ifdef USE_FILELOADER
    #include "hw/FileLoader.h"
#endif

#ifdef USE_AUDIOMANAGER
    /** @note AudioManager vyžaduje FileLoader, ten je inkludován automaticky. */
    #ifndef USE_FILELOADER
        #include "hw/FileLoader.h"
    #endif
    #include "hw/AudioManager.h"
#endif

// ============ UI (Grafické rozhraní a systém) ============
#ifdef USE_MAINSCREEN
    #include "baseUI/IAppModule.h"
    #include "baseUI/MainScreen.h"
    #include "baseUI/Settings.h"
#endif

#endif // TOOLS_H