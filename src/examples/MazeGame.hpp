#pragma once
#include <LovyanGFX.hpp>
#include "touch/Joystick.h"
#include "touch/Button.h"
#include "levels.h"
#include "hw/AudioManager.h"
#include "hw/FileLoader.h"
#include "baseUI/IAppModule.h"

class MazeGame : public IAppModule {
public:
    MazeGame(LGFX &tft, FileLoader &loader, AudioManager &music)
        : tft(tft),
          backButton(20, MAZE_HEIGHT*20 + 10, 100, 40, "Back", tft, TFT_RED, TFT_WHITE),
          joystick(700, 380, 70, 20, tft, TFT_BLACK),
          fileLoader(loader),
          musicManager(music),
          currentLevel(0),
          inGame(false),
          newLevelSoundPlayed(false)
    {}

    void startGame() {
        inGame = true;
        currentLevel = 0;
        playerX = 1;
        playerY = 1;
        drawLevel();

        //musicManager.setVolume(21);
        musicManager.playMusic("/RatMouse.mp3");
    }

    void update() {
        if (!inGame) return;

        musicManager.handleAudio();

        int tx, ty;
        if (tft.getTouch(&tx, &ty)) {
            if (backButton.isTouched()) {
                musicManager.stopMusic();
                inGame = false;
                return;
            } else if (joystick.isJoystickTouched(tx, ty)) {
                joystick.moveJoystick(tx, ty);
            }
        } else {
            joystick.updateAutoCenter();
        }

        movePlayer();
    }

    const char* getName() const override { return "RatMaze"; } // NOVÉ
    const char* getIconPath() const override { return "/icons/maze.bmp"; }
    void run() override { startGame(); }
    bool isActive() const override { return inGame; }

private:
    LGFX &tft;
    Button backButton;
    Joystick joystick;
    FileLoader &fileLoader;
    AudioManager &musicManager;

    int playerX, playerY;
    int currentLevel;
    bool inGame;
    bool newLevelSoundPlayed;

    void drawLevel() {
        tft.fillScreen(TFT_BLACK);
        const bool (*lvl)[MAZE_WIDTH] = levels[currentLevel];
        for (int y = 0; y < MAZE_HEIGHT; y++)
            for (int x = 0; x < MAZE_WIDTH; x++)
                if (lvl[y][x])
                    tft.fillRect(x*20, y*20, 20, 20, TFT_WHITE);

        drawGoal();
        drawLevelNumber();
        backButton.draw();
        joystick.drawJoystick();
        drawPlayer();

        if (!newLevelSoundPlayed && currentLevel > 0) {
            newLevelSoundPlayed = true;
        }
    }

    void drawLevelNumber() {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.setCursor(MAZE_WIDTH*20+5, 10);
        tft.printf("Level %d/5", currentLevel+1);
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
        if (millis() - lastMove < 50) return;
        lastMove = millis();

        int jx = joystick.getJoystickX() - 700;
        int jy = joystick.getJoystickY() - 380;
        int dx = 0, dy = 0;

        if (abs(jx) > 15) dx = (jx > 0 ? 1 : -1);
        if (abs(jy) > 15) dy = (jy > 0 ? 1 : -1);

        int newX = playerX + dx;
        int newY = playerY + dy;

        if (canMoveTo(newX, newY)) {
            tft.fillCircle(playerX*20 + 10, playerY*20 + 10, 8, TFT_BLACK);
            playerX = newX;
            playerY = newY;
            drawPlayer();
        }

        if (playerX == MAZE_WIDTH-2 && playerY == MAZE_HEIGHT-2) {
            nextLevel();
        }
    }

    void nextLevel() {
        currentLevel++;
        newLevelSoundPlayed = false;

        if (currentLevel >= 5) {
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_WHITE);
            tft.setTextSize(3);
            tft.setCursor(100, 200);
            tft.println("YOU WIN!");
            delay(2000);
            musicManager.stopMusic();
            inGame = false;
            return;
        }
        playerX = 1;
        playerY = 1;
        drawLevel();
    }
};
