#ifndef TOOLS_H
#define TOOLS_H

#include "hw/gfx_conf.h"
#include <LovyanGFX.hpp>

// ============ TOUCH ============
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

// ============ HW ============
#ifdef USE_FILELOADER
    #include "hw/FileLoader.h"
#endif

#ifdef USE_AUDIOMANAGER
    #ifndef USE_FILELOADER
        #include "hw/FileLoader.h"
    #endif
    #include "hw/AudioManager.h"
#endif

// ============ UI ============
#ifdef USE_MAINSCREEN
    #include "baseUI/IAppModule.h"
    #include "baseUI/MainScreen.h"
    #include "baseUI/Settings.h"
#endif

#endif
