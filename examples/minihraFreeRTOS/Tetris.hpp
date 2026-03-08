#pragma once
#define USE_BUTTON
#define USE_AUDIOMANAGER
#define USE_IAPPMODULE
#include "tools.h"

class GameTetris : public IAppModule {
public:
    GameTetris(LGFX &tft, AudioManager &music)
    : tft(tft),
      musicManager(music),
      btnLeft(20, 400, 100, 60, "Left", tft),    
      btnRight(130, 400, 100, 60, "Right", tft), 
      btnDown(565, 400, 100, 60, "Down", tft),    
      btnRotate(680, 400, 100, 60, "Rotate", tft), 
      btnBack(650, 20, 100, 40, "Back", tft),     
      active(false),
      score(0) 
    {
        randomSeed(millis());
        reset();
    }

    const char* getName() const override { return "Tetris"; }
    const char* getIconPath() const override { return "/icons/tetris.bmp"; }
    void run() override { startGame(); }
    bool isActive() const override { return active; }

    void startGame() {
        active = true;
        reset();
        drawScreen();
        drawButtons();
        musicManager.playMusic("/tetrisSong.mp3");
    }

    void update() {
        if (!active) return;

        // Kontrola tlačítka zpět
        if (btnBack.isTouched()) {
            exitGame();
            return;
        }

        unsigned long now = millis();

        // --- NEBLOKUJÍCÍ OVLÁDÁNÍ ---
        // Každý směr má vlastní časovač, aby se předešlo blokování Tasku

        if (btnLeft.isTouched()) {
            if (now - lastLeftPress > 120) { // Debounce/Repeat delay
                if (move(-1, 0)) drawBoard();
                lastLeftPress = now;
            }
        }

        if (btnRight.isTouched()) {
            if (now - lastRightPress > 120) {
                if (move(1, 0)) drawBoard();
                lastRightPress = now;
            }
        }

        if (btnDown.isTouched()) {
            if (now - lastDownPress > 70) { // Rychlejší pád při držení
                if (!move(0, 1)) {
                    handleLanding();
                }
                drawBoard();
                lastDownPress = now;
            }
        }

        if (btnRotate.isTouched()) {
            if (now - lastRotatePress > 200) {
                rotatePiece();
                drawBoard();
                lastRotatePress = now;
            }
        }

        // Automatický pád podle obtížnosti
        if (now - lastUpdate > (unsigned long)fallDelay) {
            lastUpdate = now;
            if (!move(0, 1)) {
                handleLanding();
            }
            drawBoard();
        }
    }

private:
    LGFX &tft;
    AudioManager &musicManager;
    Button btnLeft, btnRight, btnRotate, btnDown, btnBack;
    bool active;
    long score;

    // Časovače pro neblokující ovládání
    unsigned long lastLeftPress = 0;
    unsigned long lastRightPress = 0;
    unsigned long lastDownPress = 0;
    unsigned long lastRotatePress = 0;

    static const int W = 10;
    static const int H = 20;
    int board[H][W] = {0};

    int pieceX, pieceY;
    int currentPiece;
    int rotation;
    unsigned long lastUpdate = 0;
    int fallDelay = 500; 

    const int shapes[7][4][4][4] = {
        {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},{{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},{{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}},
        {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}},
        {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},{{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},{{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},{{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},{{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},{{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},{{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}}},
        {{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},{{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}},{{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},{{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},{{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},{{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}},
        {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},{{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},{{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}}
    };

    void handleLanding() {
        placePiece(); 
        if (clearLines()) {
            drawBoard();
            drawScore();
        }
        spawnPiece();
        if (collides()) showGameOver();
    }
    
    void reset() {
        memset(board, 0, sizeof(board));
        score = 0;
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
        int linesCleared = 0; 
        for(int y=H-1;y>=0;y--){
            bool full = true;
            for(int x=0;x<W;x++)
                if(!board[y][x]) { full=false; break; }
            if(full){
                anyCleared = true;
                linesCleared++; 
                for(int yy=y;yy>0;yy--)
                    for(int x=0;x<W;x++)
                        board[yy][x] = board[yy-1][x];
                for(int x=0;x<W;x++)
                    board[0][x]=0;
                y++;
            }
        }
        
        if (linesCleared > 0) {
            if (linesCleared == 1) score += 100;
            else if (linesCleared == 2) score += 300;
            else if (linesCleared == 3) score += 500;
            else if (linesCleared >= 4) score += 800;
        }
        return anyCleared;
    }

    uint16_t getColor(int n) {
        static uint16_t colors[8] = {
            TFT_BLACK, TFT_CYAN, TFT_YELLOW, TFT_MAGENTA,
            TFT_BLUE, TFT_GREEN, TFT_RED, TFT_ORANGE
        };
        return colors[n % 8];
    }
    
    void drawScore() {
        tft.fillRect(10, 30, 250, 100, TFT_BLACK); 
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(20, 40);
        tft.println("SCORE:");
        tft.setCursor(30, 70);
        tft.setTextSize(3);
        tft.printf("%ld", score);
    }
    
    void drawBoard() {
        int startX = 300, startY = 60;
        int blockSize = 18;
        
        for(int y=0;y<H;y++) {
            for(int x=0;x<W;x++) {
                uint16_t color = board[y][x] ? getColor(board[y][x]) : TFT_BLACK;
                tft.fillRect(startX+x*blockSize, startY+y*blockSize, blockSize-1, blockSize-1, color);
                if(!board[y][x]) tft.drawRect(startX+x*blockSize, startY+y*blockSize, blockSize-1, blockSize-1, 0x2104);
            }
        }

        for(int y=0;y<4;y++) {
            for(int x=0;x<4;x++) {
                if(shapes[currentPiece][rotation][y][x]) {
                    int nx = pieceX + x;
                    int ny = pieceY + y;
                    if(ny>=0) {
                        tft.fillRect(startX+nx*blockSize, startY+ny*blockSize, blockSize-1, blockSize-1, getColor(currentPiece+1));
                    }
                }
            }
        }
    }

    void drawScreen() {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(350, 20);
        tft.println("TETRIS");
        drawScore(); 
        drawBoard();
    }

    void drawButtons() {
        btnLeft.draw();
        btnRight.draw();
        btnDown.draw();
        btnRotate.draw();
        btnBack.draw();
    }
    
    void showGameOver() {
        musicManager.stopMusic();
        tft.fillRect(250, 180, 300, 120, TFT_RED);
        tft.drawRect(250, 180, 300, 120, TFT_WHITE);
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(3);
        tft.setCursor(285, 205);
        tft.println("GAME OVER");
        tft.setTextSize(2);
        tft.setCursor(310, 245);
        tft.printf("Score: %ld", score); 
        
        vTaskDelay(pdMS_TO_TICKS(3000));
        exitGame();
    }

    void exitGame() {
        musicManager.stopMusic();
        vTaskDelay(pdMS_TO_TICKS(100)); // Dejte audio tasku čas na zavření souboru
        active = false;
    }
};