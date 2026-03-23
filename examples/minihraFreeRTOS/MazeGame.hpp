#pragma once

#define USE_JOYSTICK
#define USE_BUTTON
#define USE_FILELOADER
#define USE_AUDIOMANAGER
#define USE_IAPPMODULE

#include "tools.h"
#include "levels.h"

class MazeGame : public IAppModule {
public:
    MazeGame(LGFX &tft, FileLoader &loader, AudioManager &music)
        : tft(tft),
          backButton(20, MAZE_HEIGHT*20 + 10, 100, 40, "Back", tft, TFT_RED, TFT_WHITE),
          joystick(700, 380, 70, 20, tft, TFT_BLACK),
          fileLoader(loader),
          musicManager(music),
          currentLevel(0),
          inGame(false)
    {}

    const char* getName() const override { return "RatMaze"; }
    void run() override { startGame(); }
    bool isActive() const override { return inGame; }

    void startGame() {
        inGame = true;
        currentLevel = 0;
        playerX = 1;
        playerY = 1;
        drawLevel();
        musicManager.playMusic("/RatMouse.mp3");
    }

    void update() {
        if (!inGame) return;

        // V RTOS verzi už handleAudio() nevoláme, běží na druhém jádře!

        int tx, ty;
        bool touched = tft.getTouch(&tx, &ty);

        if (touched) {
            if (backButton.isTouched()) {
                exitGame();
                return;
            } 
            // Ovládání virtuálního joysticku
            joystick.moveJoystick(tx, ty);
        } else {
            joystick.updateAutoCenter();
        }

        // Logika pohybu a vítězství
        movePlayer();

        // Krátká pauza pro stabilitu UI tasku
        vTaskDelay(pdMS_TICKS_TO_MS(10));
    }

private:
    LGFX &tft;
    Button backButton;
    Joystick joystick;
    FileLoader &fileLoader;
    AudioManager &musicManager;

    int playerX, playerY;
    int currentLevel;
    bool inGame;

    void exitGame() {
        musicManager.stopMusic();
        vTaskDelay(pdMS_TO_TICKS(100));
        inGame = false;
    }

    void drawLevel() {
        tft.fillScreen(TFT_BLACK);
        
        // Vykreslení bludiště
        for (int y = 0; y < MAZE_HEIGHT; y++) {
            for (int x = 0; x < MAZE_WIDTH; x++) {
                if (levels[currentLevel][y][x]) {
                    tft.fillRect(x*20, y*20, 20, 20, TFT_WHITE);
                }
            }
        }

        drawGoal();
        drawLevelNumber();
        backButton.draw();
        joystick.drawJoystick();
        drawPlayer();
    }

    void drawLevelNumber() {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(MAZE_WIDTH*20 + 10, 10);
        tft.printf("Level %d/5", currentLevel + 1);
    }

    void drawPlayer() {
        tft.fillCircle(playerX*20 + 10, playerY*20 + 10, 8, TFT_RED);
    }

    void drawGoal() {
        tft.fillRect((MAZE_WIDTH-2)*20, (MAZE_HEIGHT-2)*20, 20, 20, TFT_GREEN);
    }

    bool canMoveTo(int x, int y) {
        if (x < 0 || y < 0 || x >= MAZE_WIDTH || y >= MAZE_HEIGHT) return false;
        return levels[currentLevel][y][x] == 0;
    }

    void movePlayer() {
        static unsigned long lastMove = 0;
        // Omezení rychlosti pohybu (tick rate bludiště)
        if (millis() - lastMove < 70) return; 
        lastMove = millis();

        // Výpočet vektoru z joysticku
        int jx = joystick.getJoystickX() - 700;
        int jy = joystick.getJoystickY() - 380;
        int dx = 0, dy = 0;

        if (abs(jx) > 20) dx = (jx > 0 ? 1 : -1);
        if (abs(jy) > 20) dy = (jy > 0 ? 1 : -1);

        if (dx != 0 || dy != 0) {
            int newX = playerX + dx;
            int newY = playerY + dy;

            if (canMoveTo(newX, newY)) {
                // Smazání staré pozice (vykreslení černé podlahy)
                tft.fillCircle(playerX*20 + 10, playerY*20 + 10, 8, TFT_BLACK);
                playerX = newX;
                playerY = newY;
                drawPlayer();
            }

            // Kontrola vítězství v levelu
            if (playerX == MAZE_WIDTH-2 && playerY == MAZE_HEIGHT-2) {
                nextLevel();
            }
        }
    }

    void nextLevel() {
        currentLevel++;

        if (currentLevel >= 5) {
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setTextSize(4);
            tft.setCursor(250, 200);
            tft.println("VICTORY!");
            
            vTaskDelay(pdMS_TICKS_TO_MS(2500)); // RTOS delay nezasekne hudbu
            exitGame();
            return;
        }

        playerX = 1;
        playerY = 1;
        drawLevel();
    }
};