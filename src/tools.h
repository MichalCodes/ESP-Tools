#ifndef TOOLS_H
#define TOOLS_H

/**
 * tools.h - Smart linking pro moduly
 */

// ============ TOUCHSCREEN / INPUT DEVICES ============
#include "hw/gfx_conf.h"
#include <LovyanGFX.hpp>

#ifdef JOYSTICK_H
    #include "touch/Joystick.h"
#endif

#ifdef SWIPECONTROLLER_H
    #include "touch/SwipeControler.h"
#endif

#ifdef BUTTON_H
    #include "touch/Button.h"
#endif

#ifdef TRACKBAR_H
    #include "touch/TrackBar.h"
#endif

// ============ HARDWARE / SYSTEM ============
#ifdef FILELOADER_H
    #include "hw/FileLoader.h"
#endif

#ifdef AUDIOMANAGER_H
    #ifndef FILELOADER_H 
        #include "hw/FileLoader.h"
    #endif
    #include "hw/AudioManager.h"
#endif

// ============ UI / SCREENS ============
#ifdef MAINSCREEN_H
    #ifndef BUTTON_H
        #include "touch/Button.h"
    #endif
    #include "baseUI/IAppModule.h"
    #include "baseUI/MainScreen.h"
    #include "baseUI/Settings.h"
#endif

#endif 
