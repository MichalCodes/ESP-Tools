#ifndef TOOLS_H
#define TOOLS_H

/**
 * tools.h - Smart linking pro wszystkie moduly
 * Podmíněné includování jednotlivých komponent na základě jejich header guard.
 * 
 * Princip chytrého linkování:
 * - Pokud je modul obsažen v zdrojovém souboru (.cpp), definuje svůj MODULENAME_H guard
 * - tools.h jej detekuje a includuje odpovídající header
 * - Závislosti se řeší automaticky (AudioManager potřebuje FileLoader)
 */

// ============ TOUCHSCREEN / INPUT DEVICES ============
#ifdef BUTTON_H
    #include "Button.h"
#endif

#ifdef JOYSTICK_H
    #include "Joystick.h"
#endif

#ifdef SWIPECONTROLLER_H
    #include "SwipeController.h"
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
    #include "MainScreen.h"
#endif

// ============ GAMES ============
#ifdef GAME2048_H
    #include "Game2048.hpp"
#endif

#ifdef TETRIS_H
    #include "Tetris.hpp"
#endif

#ifdef MAZEGAME_H
    #include "MazeGame.hpp"
#endif

#endif 
