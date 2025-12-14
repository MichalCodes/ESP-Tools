#pragma once
#include <LovyanGFX.hpp>
#include "Button.h"
#include "SwipeControler.h"
#include "IAppModule.h"

class Game2048 : public IAppModule {
public:
    Game2048(LGFX &tft)
    : tft(tft),
      backButton(450, 400, 200, 50, "Back", tft),
      swipe(tft),
      active(false),
      score(0)
    {
        resetBoard();
    }

    bool isActive() const override { return active; }
    void exitGame() { active = false; }
    void run() override { drawScreen(); }
    const char* getName() const override { return "2048"; }
    const char* getIconPath() const override { return "/icons/2048.bmp"; }

    void drawScreen() {
        active = true;
        tft.fillScreen(TFT_BLUE);
        tft.fillRect(0, 0, 800, 70, TFT_GREEN);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(50, 30);
        tft.println("2048 Game");

        backButton.draw();
        resetBoard();
        drawTiles();
        drawScore();
    }

    void update() {
        if(!active) return;

        int touchX, touchY;
        swipe.update();

        if(tft.getTouch(&touchX, &touchY)){
            if(backButton.isTouched()){
                exitGame();
                return;
            }
        }

        handleSwipe();
    }

private:
    LGFX &tft;
    Button backButton;
    SwipeController swipe;
    bool active;
    int score;

    static const int N = 4;
    int board[N][N];
    bool newBlock = false;

    // --- herní logika ---
    void resetBoard() {
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                board[i][j] = 0;

        addRandomTile();
        addRandomTile();
        score = 0;
    }

    void addRandomTile() {
        int emptyCount = 0;
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                if(board[i][j] == 0) emptyCount++;

        if(emptyCount == 0) return;

        int r = random(emptyCount);
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(board[i][j] == 0){
                    if(r == 0) {
                        board[i][j] = (random(10) < 9) ? 2 : 4;
                        return;
                    }
                    r--;
                }
            }
        }
    }

    bool moveLeft() {
        bool moved = false;
        for(int i=0;i<N;i++){
            int target = 0;
            int lastMerged = -1;
            for(int j=0;j<N;j++){
                if(board[i][j]==0) continue;
                int val = board[i][j];
                board[i][j]=0;
                if(target>0 && board[i][target-1]==val && lastMerged != target-1){
                    board[i][target-1]*=2;
                    score += board[i][target-1];
                    lastMerged = target-1;
                    moved = true;
                } else {
                    if(j!=target) moved = true;
                    board[i][target++] = val;
                }
            }
        }
        return moved;
    }

    bool moveRight() {
        bool moved = false;
        for(int i=0;i<N;i++){
            int target = N-1;
            int lastMerged = N;
            for(int j=N-1;j>=0;j--){
                if(board[i][j]==0) continue;
                int val = board[i][j];
                board[i][j]=0;
                if(target<N-1 && board[i][target+1]==val && lastMerged != target+1){
                    board[i][target+1]*=2;
                    score += board[i][target+1];
                    lastMerged = target+1;
                    moved = true;
                } else {
                    if(j!=target) moved = true;
                    board[i][target--] = val;
                }
            }
        }
        return moved;
    }

    bool moveUp() {
        bool moved = false;
        for(int j=0;j<N;j++){
            int target = 0;
            int lastMerged = -1;
            for(int i=0;i<N;i++){
                if(board[i][j]==0) continue;
                int val = board[i][j];
                board[i][j]=0;
                if(target>0 && board[target-1][j]==val && lastMerged != target-1){
                    board[target-1][j]*=2;
                    score += board[target-1][j];
                    lastMerged = target-1;
                    moved = true;
                } else {
                    if(i!=target) moved = true;
                    board[target++][j] = val;
                }
            }
        }
        return moved;
    }

    bool moveDown() {
        bool moved = false;
        for(int j=0;j<N;j++){
            int target = N-1;
            int lastMerged = N;
            for(int i=N-1;i>=0;i--){
                if(board[i][j]==0) continue;
                int val = board[i][j];
                board[i][j]=0;
                if(target<N-1 && board[target+1][j]==val && lastMerged != target+1){
                    board[target+1][j]*=2;
                    score += board[target+1][j];
                    lastMerged = target+1;
                    moved = true;
                } else {
                    if(i!=target) moved = true;
                    board[target--][j] = val;
                }
            }
        }
        return moved;
    }

    bool hasMoves() {
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                if(board[i][j]==0) return true;

        for(int i=0;i<N;i++)
            for(int j=0;j<N-1;j++)
                if(board[i][j]==board[i][j+1] || board[j][i]==board[j+1][i]) return true;

        return false;
    }

    void handleSwipe() {
        SwipeController::Swipe s = swipe.getSwipe();
        if(s == SwipeController::NONE) return;

        bool moved = false;
        switch(s) {
            case SwipeController::LEFT:  moved = moveLeft();  break;
            case SwipeController::RIGHT: moved = moveRight(); break;
            case SwipeController::UP:    moved = moveUp();    break;
            case SwipeController::DOWN:  moved = moveDown();  break;
            default: break;
        }

        if(moved) {
            addRandomTile();
            drawTiles();
            drawScore();

            if(!hasMoves()) {
                showGameOver();
            }
        }
    }

    uint16_t getTileColor(int value) {
        switch(value) {
            case 0:   return TFT_DARKGREY; // prázdné pole
            case 2:   return 0xFFFF;       // světle šedá
            case 4:   return 0xFFFF;       // béžová
            case 8:   return 0xFDB8;       // pleťová
            case 16:  return 0xFD20;       // světle oranžová
            case 32:  return 0xFC60;       // tmavší oranžová
            case 64:  return 0xF800;       // červená
            case 128: return 0xFFE0;       // světle žlutá
            case 256: return 0xFFE0;       // světle žlutá
            case 512: return 0xFFE0;       // světle žlutá
            default:  return 0xFFC0;       // sytě žlutá (1024+)
        }
    }

    void drawTiles() {
        int startX = 50;
        int startY = 100;
        int tileSize = 80;
        int spacing = 10;

        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                int x = startX + j * (tileSize + spacing);
                int y = startY + i * (tileSize + spacing);

                uint16_t color = getTileColor(board[i][j]);
                tft.fillRoundRect(x, y, tileSize, tileSize, 8, color);
                tft.drawRoundRect(x, y, tileSize, tileSize, 8, TFT_BLACK);

                if(board[i][j] != 0){
                    tft.setTextColor((board[i][j] <= 4) ? TFT_BLACK : TFT_WHITE);
                    tft.setTextSize(2);
                    int textX = x + tileSize/2 - 10;
                    int textY = y + tileSize/2 - 8;
                    tft.setCursor(textX, textY);
                    tft.printf("%d", board[i][j]);
                }
            }
        }

        // vykresli score vedle herního pole
        drawScore();
    }

    void drawScore() {
        int x = 450;  // pozice napravo od herního pole
        int y = 150;

        tft.fillRect(x, y, 300, 150, TFT_GREEN);
        tft.drawRect(x, y, 300, 150, TFT_BLACK);

        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(3);
        tft.setCursor(x + 30, y + 50);
        tft.printf("SCORE");
        tft.setTextSize(4);
        tft.setCursor(x + 40, y + 100);
        tft.printf("%d", score);
    }


    void showGameOver() {
        tft.fillRect(250, 180, 300, 120, TFT_RED);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(300, 220);
        tft.println("GAME OVER");
    }
};
