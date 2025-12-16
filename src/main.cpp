#include <Wire.h>
#include <SPI.h>

#define USE_JOYSTICK
#define USE_BUTTON
#define USE_TRACKBAR
#define USE_FILELOADER
#define USE_AUDIOMANAGER
#define USE_MAINSCREEN

#include "tools.h"
#include "examples/Game2048.hpp"
#include "examples/Tetris.hpp"
#include "examples/MazeGame.hpp"

FileLoader fileLoader;
AudioManager musicManager(fileLoader);
MazeGame mazeGame(tft, fileLoader, musicManager); 
Game2048 game2048(tft);
GameTetris tetris(tft, musicManager);
SettingsManager settingsManager(musicManager); 
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
    
    mainMenu.addModule(game2048);
    mainMenu.addModule(tetris); 
    mainMenu.addModule(mazeGame);
    
    mainMenu.draw();
}

void loop() {
    musicManager.handleAudio();

    if (settingsManager.isActive()) {
        settingsManager.update();
        if (!settingsManager.isActive()) {
            mainMenu.draw();
        }
        
        return; 
    }

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

    mainMenu.handleTouch();
}