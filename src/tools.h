#ifndef TOOLS_H
#define TOOLS_H

/**
 * tools.h - Smart linking pro moduly
 */

// ============ TOUCHSCREEN / INPUT DEVICES ============
#include "hw/gfx_conf.h"
#include <LovyanGFX.hpp>

#ifdef JOYSTICK_H
    #include "Joystick.h"
#endif

#ifdef SWIPECONTROLLER_H
    #include "SwipeController.h"
#endif

#ifdef BUTTON_H
    #include "Button.h"
#endif

#ifdef TRACKBAR_H
    #include "TrackBar.h"
#endif

// ============ HARDWARE / SYSTEM ============
#ifdef FILELOADER_H
    #include "FileLoader.h"
#endif

#ifdef AUDIOMANAGER_H
    #ifndef FILELOADER_H 
        #include "FileLoader.h"
    #endif
    #include "AudioManager.h"
#endif

// ============ UI / SCREENS ============
#ifdef MAINSCREEN_H
    #ifndef BUTTON_H
        #include "Button.h"
    #endif
    #include "IAppModule.h"
    #include "MainScreen.h"
    #include "Settings.h"
#endif

#endif 
