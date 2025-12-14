#pragma once
#include <LovyanGFX.hpp>
#include "Button.hpp"
#include "AudioManager.h"  // tvůj MusicManager

class GameTetris {
public:
    GameTetris(LGFX &tft, AudioManager &music)
    : tft(tft),
      musicManager(music),
      btnLeft(50, 400, 80, 60, "L", tft),
      btnRight(650, 400, 100, 60, "R", tft),
      btnRotate(500, 400, 100, 60, "Rot", tft),
      btnDown(200, 400, 80, 60, "Down", tft),
      btnBack(650, 20, 100, 40, "Back", tft),
      active(false)
    {
        randomSeed(millis());
        reset();
    }

    bool isActive() const { return active; }

    void startGame() {
        active = true;
        reset();
        drawScreen();
        drawButtons();

        // hlavní hudba
        //musicManager.setVolume(21);
        musicManager.playMusic("/tetrisSong.mp3");
    }

    void update() {
        if (!active) return;

        // ---- ovládací tlačítka ----
        if (btnBack.isTouched()) {
            exitGame();
            return;
        }

        if (btnLeft.isTouched()) {
            move(-1, 0);
            drawBoard();
            delay(150);
        }

        if (btnRight.isTouched()) {
            move(1, 0);
            drawBoard();
            delay(150);
        }

        if (btnDown.isTouched()) {
            dropPiece();
            drawBoard();
            delay(150);
        }

        if (btnRotate.isTouched()) {
            rotatePiece();
            drawBoard();
            delay(150);
        }

        // ---- gravitační posun ----
        if (millis() - lastUpdate > fallDelay) {
            lastUpdate = millis();
            if (!move(0, 1)) {
                dropPiece(); // dopadl kus -> SFX uvnitř dropPiece()
            }
            drawBoard();
        }

        // loop hudby a SFX
        musicManager.handleAudio();
    }

private:
    LGFX &tft;
    AudioManager &musicManager;
    Button btnLeft, btnRight, btnRotate, btnDown, btnBack;
    bool active;

    static const int W = 10;
    static const int H = 20;
    int board[H][W] = {0};

    int pieceX, pieceY;
    int currentPiece;
    int rotation;
    unsigned long lastUpdate = 0;
    int fallDelay = 500; // ms

    // všechny tvary kostek
    const int shapes[7][4][4][4] = {
        // I
        {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
         {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
         {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
         {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
        // O
        {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
         {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
         {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
         {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}},
        // T
        {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
         {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
         {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
         {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}},
        // L
        {{{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
         {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
         {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
         {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}},
        // J
        {{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
         {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}},
         {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
         {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}},
        // S
        {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
         {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
         {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
         {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}},
        // Z
        {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
         {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
         {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
         {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}}
    };

    void reset() {
        memset(board, 0, sizeof(board));
        spawnPiece();
    }

    void spawnPiece() {
        currentPiece = random(7);
        rotation = 0;
        pieceX = W/2 - 2;
        pieceY = 0;
    }

    bool collides(int dx=0, int dy=0, int rot=-1) {
        int r = (rot == -1) ? rotation : rot;
        for(int y=0;y<4;y++)
            for(int x=0;x<4;x++)
                if(shapes[currentPiece][r][y][x]) {
                    int nx = pieceX + x + dx;
                    int ny = pieceY + y + dy;
                    if(nx < 0 || nx >= W || ny >= H) return true;
                    if(ny >= 0 && board[ny][nx]) return true;
                }
        return false;
    }

    bool move(int dx, int dy) {
        if(!collides(dx, dy)) {
            pieceX += dx;
            pieceY += dy;
            return true;
        }
        return false;
    }

    void rotatePiece() {
        int newRot = (rotation + 1) % 4;
        if(!collides(0, 0, newRot)) rotation = newRot;
    }

    void dropPiece() {
        while(move(0,1));
        placePiece();
        spawnPiece();
        if (collides()) showGameOver();
    }

    void placePiece() {
        for(int y=0;y<4;y++)
            for(int x=0;x<4;x++)
                if(shapes[currentPiece][rotation][y][x]) {
                    int nx = pieceX + x;
                    int ny = pieceY + y;
                    if(ny>=0 && ny<H && nx>=0 && nx<W)
                        board[ny][nx] = currentPiece+1;
                }
    }

    bool clearLines() {
        bool anyCleared = false;
        for(int y=H-1;y>=0;y--){
            bool full = true;
            for(int x=0;x<W;x++)
                if(!board[y][x]) { full=false; break; }
            if(full){
                anyCleared = true;
                for(int yy=y;yy>0;yy--)
                    for(int x=0;x<W;x++)
                        board[yy][x] = board[yy-1][x];
                for(int x=0;x<W;x++)
                    board[0][x]=0;
                y++;
            }
        }
        return anyCleared;
    }

    void drawScreen() {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(300, 20);
        tft.println("TETRIS");
        drawBoard();
    }

    void drawButtons() {
        btnLeft.draw();
        btnRight.draw();
        btnDown.draw();
        btnRotate.draw();
        btnBack.draw();
    }

    void drawBoard() {
        int startX = 300, startY = 60;
        int blockSize = 20;
        tft.fillRect(startX-2, startY-2, W*blockSize+4, H*blockSize+4, TFT_BLACK);

        for(int y=0;y<H;y++)
            for(int x=0;x<W;x++)
                if(board[y][x])
                    tft.fillRect(startX+x*blockSize, startY+y*blockSize, blockSize-1, blockSize-1, getColor(board[y][x]));
                else
                    tft.drawRect(startX+x*blockSize, startY+y*blockSize, blockSize-1, blockSize-1, TFT_DARKGREY);

        for(int y=0;y<4;y++)
            for(int x=0;x<4;x++)
                if(shapes[currentPiece][rotation][y][x]) {
                    int nx = pieceX + x;
                    int ny = pieceY + y;
                    if(ny>=0)
                        tft.fillRect(startX+nx*blockSize, startY+ny*blockSize, blockSize-1, blockSize-1, getColor(currentPiece+1));
                }
    }

    uint16_t getColor(int n) {
        static uint16_t colors[8] = {
            TFT_BLACK, TFT_CYAN, TFT_YELLOW, TFT_MAGENTA,
            TFT_BLUE, TFT_GREEN, TFT_RED, TFT_ORANGE
        };
        return colors[n % 8];
    }

    void showGameOver() {
        musicManager.stopMusic();
        tft.fillRect(250, 200, 300, 100, TFT_RED);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(320, 240);
        tft.println("GAME OVER");
        delay(1500);
        exitGame();
    }

    void exitGame() {
        musicManager.stopMusic();
        active = false;
    }
};
