#include <Wire.h>
#include <SPI.h>
#include "gfx_conf.h" 
#include "FileLoader.h"
#include "AudioManager.h" 
#include "Settings.h" 
#include "MainScreen.h" 

// Původní hry (nyní se stanou moduly)
#include "Game2048.hpp"
#include "Tetris.hpp"
#include "MazeGame.hpp"

// Nové rozhraní
#include "IAppModule.h" 
// Obalové třídy jako Game2048Module.hpp již nejsou potřeba!


// ---------- Globální objekty ----------
FileLoader fileLoader;
AudioManager musicManager(fileLoader);

// Původní instance her
MazeGame mazeGame(tft, fileLoader, musicManager); 
Game2048 game2048(tft);
GameTetris tetris(tft, musicManager);

SettingsManager settingsManager(musicManager); 

// NOVÝ OBJEKT: Hlavní obrazovka - Nastavíme titulek!
// FIX: Odstraněny nepotřebné reference na hry z konstruktoru
MainScreen mainMenu(tft, "ESP Minigame", settingsManager);


void setup() {
    Serial.begin(115200);
    delay(2000); 

    tft.begin();
    tft.setTextDatum(0); 

    if (!fileLoader.begin()) {
        Serial.println("SD karta nedostupná!");
    } else {
        fileLoader.listFiles("/", 2); 
    }

    musicManager.begin();
    
    // --- Nastavení modulů ---
    // FIX: Nyní přidáváme instance hry přímo, protože implementují IAppModule
    mainMenu.addModule(game2048);
    mainMenu.addModule(tetris); 
    mainMenu.addModule(mazeGame);
    
    mainMenu.draw();
}

void loop() {
    musicManager.handleAudio();

    if (settingsManager.isActive()) {
        
        if (!settingsManager.isActive()) {
            mainMenu.draw();
        }
        
        return; 
    }

    // Zpracování her: Nyní všechny používají isActive()
    if (game2048.isActive()) { 
        game2048.update();
        if (!game2048.isActive()) mainMenu.draw(); 
        return;
    } 

    if (tetris.isActive()) {
        tetris.update();
        if (!tetris.isActive()) mainMenu.draw(); 
        return;
    }

    if (mazeGame.isActive()) {
        mazeGame.update();
        if (!mazeGame.isActive()) mainMenu.draw(); 
        return;
    }

    // 4. Hlavní menu (Spouštění her a nastavení přes ikony)
    mainMenu.handleTouch();
}