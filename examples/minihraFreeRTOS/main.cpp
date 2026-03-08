#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

// Definice pro tools.h
#define USE_JOYSTICK
#define USE_BUTTON
#define USE_TRACKBAR
#define USE_FILELOADER
#define USE_AUDIOMANAGER
#define USE_MAINSCREEN

#include "tools.h"

// Oprava makra pro FreeRTOS
#ifndef pdMS_TICKS_TO_MS
#define pdMS_TICKS_TO_MS(ms) pdMS_TO_TICKS(ms)
#endif

#include "../examples/minihraFreeRTOS/Game2048.hpp"
#include "../examples/minihraFreeRTOS/Tetris.hpp"
#include "../examples/minihraFreeRTOS/MazeGame.hpp"

// Globální instance
FileLoader fileLoader;
AudioManager musicManager(fileLoader);
MazeGame mazeGame(tft, fileLoader, musicManager); 
Game2048 game2048(tft);
GameTetris tetris(tft, musicManager);
SettingsManager settingsManager(musicManager); 
MainScreen mainMenu(tft, "ESP Minigame", settingsManager);

// Synchronizační zámek pro sdílenou sběrnici SPI
SemaphoreHandle_t xHardwareSemaphore = NULL;

TaskHandle_t xMusicTaskHandle = NULL;
TaskHandle_t xGameTaskHandle = NULL;

// --- TASK PRO AUDIO (Core 0) ---
void musicTask(void *pvParameters) {
    while (1) {
        // Získáme zámek jen na krátkou dobu obsluhy audia
        if (xSemaphoreTake(xHardwareSemaphore, pdMS_TO_TICKS(5)) == pdTRUE) {
            musicManager.handleAudio();
            xSemaphoreGive(xHardwareSemaphore);
        }
        vTaskDelay(1); 
    }
}

// --- TASK PRO UI A HRY (Core 1) ---
void uiTask(void *pvParameters) {
    bool lastStateWasGame = false;
    
    // Prvotní vykreslení menu
    if (xSemaphoreTake(xHardwareSemaphore, portMAX_DELAY) == pdTRUE) {
        mainMenu.draw();
        xSemaphoreGive(xHardwareSemaphore);
    }

    while (1) {
        bool anyGameActive = game2048.isActive() || tetris.isActive() || mazeGame.isActive() || settingsManager.isActive();

        // 1. LOGIKA HRY (Výpočty, vstupy) - Probíhá BEZ zámku, aby MusicTask mohl hrát
        if (anyGameActive) {
            if (settingsManager.isActive()) settingsManager.update();
            else if (game2048.isActive()) game2048.update();
            else if (tetris.isActive()) tetris.update();
            else if (mazeGame.isActive()) mazeGame.update();
            lastStateWasGame = true;
        } 
        else {
            // Jsme v menu - musíme vzít zámek pro handleTouch (protože handleTouch i kreslí)
            if (xSemaphoreTake(xHardwareSemaphore, pdMS_TO_TICKS(10)) == pdTRUE) {
                if (lastStateWasGame) {
                    musicManager.stopMusic(); 
                    tft.fillScreen(TFT_BLACK);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    mainMenu.draw();
                    lastStateWasGame = false;
                }
                mainMenu.handleTouch();
                xSemaphoreGive(xHardwareSemaphore);
            }
        }

        // Malá pauza pro stabilitu (UI běží na cca 50Hz)
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void setup() {
    Serial.begin(115200);
    
    xHardwareSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xHardwareSemaphore); 

    tft.begin();
    tft.setTextDatum(0); 

    if (!fileLoader.begin()) {
        Serial.println("SD karta nedostupna!");
    }

    musicManager.begin();
    
    mainMenu.addModule(game2048);
    mainMenu.addModule(tetris); 
    mainMenu.addModule(mazeGame);
    
    xTaskCreatePinnedToCore(musicTask, "MusicTask", 4096, NULL, 3, &xMusicTaskHandle, 0);
    xTaskCreatePinnedToCore(uiTask, "UITask", 8192, NULL, 1, &xGameTaskHandle, 1);
}

void loop() {
    vTaskDelete(NULL); 
}