#include <Wire.h>
#include <SPI.h>
#include "gfx_conf.h" 
#include "Game2048.hpp"
#include "Tetris.hpp"
#include "MazeGame.hpp"
#include "Button.hpp"
#include "SwipeControler.hpp"
#include "Joystick.h"
#include "FileLoader.hpp"
#include "AudioManager.h" 
#include "Settings.hpp" 
#include "MainScreen.hpp" 


FileLoader fileLoader;
AudioManager musicManager(fileLoader);

MazeGame mazeGame(tft, fileLoader, musicManager); 
Game2048 game2048(tft);
GameTetris tetris(tft, musicManager);

SettingsManager settingsManager(musicManager); 

MainScreen mainMenu(tft, game2048, tetris, mazeGame, settingsManager);


void setup() {
    Serial.begin(115200);
    delay(2000); 

    Serial.printf("Celková velikost PSRAM: %d bytů\n", ESP.getPsramSize());
    Serial.printf("Volná PSRAM: %d bytů\n", ESP.getFreePsram());
    tft.begin();
    tft.setTextDatum(0); 

    if (!fileLoader.begin()) {
        Serial.println("SD karta nedostupná!");
    } else {
        fileLoader.listFiles("/", 2); 
    }

    musicManager.begin();

    mainMenu.draw();
}

void loop() {
    musicManager.handleAudio();

    if (settingsManager.isActive()) {
        int touchX, touchY;
        
        settingsManager.handleTouch(); 
        
        if (tft.getTouch(&touchX, &touchY) && mainMenu.isSettingsButtonTouched()) { 
            settingsManager.hide();
            mainMenu.draw(); 
        }
        
        return; 
    }

    if (game2048.isInGame()) {
        game2048.update();
        if (!game2048.isInGame()) mainMenu.draw(); 
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