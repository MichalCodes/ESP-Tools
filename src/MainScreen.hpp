#ifndef MAIN_SCREEN_HPP
#define MAIN_SCREEN_HPP

#include "gfx_conf.h" // Pro LGFX tft
#include "Button.hpp"
#include "Game2048.hpp" // Pro typy her
#include "Tetris.hpp"
#include "MazeGame.hpp"
#include "Settings.hpp" // Pro SettingsManager

// --- Definuje možné akce, které Loop() provede na základě dotyku ---
enum class MainScreenAction {
    NONE,
    START_GAME_2048,
    START_TETRIS,
    START_MAZE_GAME,
    OPEN_SETTINGS
};

class MainScreen {
private:
    // Reference na LGFX objekt pro kreslení
    LGFX& tft;

    // Tlačítka
    Button game2048Button;
    Button tetrisButton;
    Button mazeGameButton;
    Button settingsButton;

    // Reference na objekty pro snadné volání jejich iniciačních metod
    Game2048& game2048;
    GameTetris& tetris;
    MazeGame& mazeGame;
    SettingsManager& settingsManager;


public:
    /**
     * @brief Konstruktor pro MainScreen.
     * @param display Odkaz na LGFX objekt (tft).
     * @param g2048 Odkaz na instanci Game2048.
     * @param ttrs Odkaz na instanci GameTetris.
     * @param maze Odkaz na instanci MazeGame.
     * @param settings Odkaz na instanci SettingsManager.
     */
    MainScreen(LGFX& display, 
               Game2048& g2048, 
               GameTetris& ttrs, 
               MazeGame& maze,
               SettingsManager& settings)
        // Inicializace referencí
        : tft(display),
          game2048(g2048),
          tetris(ttrs),
          mazeGame(maze),
          settingsManager(settings),
          // Inicializace tlačítek s jejich pozicemi a texty
          game2048Button(50, 100, 200, 80, "2048", tft),
          tetrisButton(550, 100, 200, 80, "Tetris", tft),
          mazeGameButton(300, 100, 200, 80, "RatMaze", tft),
          settingsButton(700, 10, 90, 50, "SET", tft, TFT_RED, TFT_WHITE)
    {}

    /**
     * @brief Vykreslí celou hlavní obrazovku s titulkem a všemi tlačítky.
     * Nahrazuje původní drawMainScreen().
     */
    void draw() {
        tft.fillScreen(TFT_BLUE);
        tft.fillRect(0, 0, 800, 70, TFT_GREEN);
        tft.setTextColor(TFT_BLACK);
        tft.setTextSize(3);
        tft.setCursor(50, 30);
        tft.println("ESP Minigame");

        // Vykreslení všech tlačítek menu
        game2048Button.draw();
        tetrisButton.draw();
        mazeGameButton.draw();
        settingsButton.draw();
    }
    
    /**
     * @brief Kontroluje, zda bylo stisknuto tlačítko Settings/Exit.
     * Tato metoda je volána z loop() v main.cpp pro opuštění nastavení.
     * @return true, pokud bylo tlačítko SET dotknuto.
     */
    bool isSettingsButtonTouched() {
        return settingsButton.isTouched();
    }

    /**
     * @brief Zpracuje dotyk na hlavní obrazovce.
     * @return Enum MainScreenAction indikující, co se má dít dál v loop().
     */
    MainScreenAction handleTouch() {
        int touchX, touchY;
        
        // Zpracování dotyku
        if (tft.getTouch(&touchX, &touchY)) {
            
            if (isSettingsButtonTouched()) { // Používáme novou metodu
                // SettingsManager by měl mít metodu .show(), která nakreslí menu
                settingsManager.show();
                return MainScreenAction::OPEN_SETTINGS;
            
            } else if (game2048Button.isTouched()) {
                // Hra by měla mít metodu .drawScreen() / .startGame()
                game2048.drawScreen(); 
                return MainScreenAction::START_GAME_2048;
                
            } else if (tetrisButton.isTouched()) {
                tetris.startGame();
                return MainScreenAction::START_TETRIS;
                
            } else if (mazeGameButton.isTouched()) {
                mazeGame.startGame();
                return MainScreenAction::START_MAZE_GAME;
            }
        }
        
        return MainScreenAction::NONE;
    }
};

#endif // MAIN_SCREEN_HPP