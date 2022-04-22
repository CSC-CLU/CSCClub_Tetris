//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_GAME_H
#define CSCCLUB_TETRIS_GAME_H
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <SDL2/SDL_ttf.h>

#include<string>

#include<cmath>
#include "Shape.h"
#include "Color.h"
#include "Bag.h"

enum class GameState
{
    START, PLAY, GAME_OVER,EXIT
};

class Game
{
public:
    Game() : Game(960,1080){};
    Game(int screenWidth, int screenHeight);
    ~Game();

    void run();

private:
    SDL_Window* window;
    int screenWidth;
    int screenHeight;
    GameState gameState;

    void initScene();
    void destructScene();
    void processInput();
    void gameLoop();
    void play();

// grid logic (game.cpp)
public:
    // todo in all honesty this could be moved entirely out of game.
    constexpr static int ROWS=20, COLS=10; // actual board dimensions
    constexpr static int PADDING=1; // minimum space between elements
    // size of the preview area
    constexpr static int PREVIEW_SIZE = Shape::N_SQUARES;
    // total amount of empty space forced to either size of the grid.
    // currently, enough to fit a preview area on either side of the grid.
    constexpr static int
        GRID_LEFT= PREVIEW_SIZE + 2 * PADDING,
        GRID_RIGHT=COLS+GRID_LEFT;

    int tileLength() const // the size of a square, which is made to be a specific proportion of the screen to ensure everything fits.
    { return (int)fmin(screenHeight/(ROWS+2*PADDING),screenWidth/(GRID_LEFT + GRID_RIGHT)); }

    // fixme this should not be needed if we are assuming we only have one window.
    // wraps sdl_rect to automatically scale by grid, honoring padding.
    struct Rect : public SDL_Rect {
        // initialized via game instance in Game.cpp.
        Rect(int x, int y, int w=1, int h=1);
    };

    // the color of a specific tile on the grid
    Color grid[COLS][ROWS];
    bool rowComplete(int y) const;

    bool moveRows(int y);

    bool clearRow(int y);
private:
    Bag bag; // for drawing pieces
    // shape logic
    Shape *nxtShape=nullptr,*curShape=nullptr, *heldShape = nullptr;
    void loadNewShape();
    void setCurShape(Shape*);
    void holdShape();
    void placeShape();
    bool moveCurShapeDown();
    // render methods (scene.cpp)
    void renderPreview(int offset, Shape*, const char*);
    void prepareScene(Color={255 / 3, 255 / 3, 255 / 3});
    void presentScene();
    // control logic (game.cpp)
    // time (ms) game waits between renders. Also the value of 1G.
    constexpr static uint32_t DELAY = 16;
    // timer logic
    int level;
    static constexpr int8_t LEVEL_CLEAR = 10;
    int toNextLevel;
    void incLevel();
    int score = 0;
    void calcScore(int rowsCleared);
    bool boardClear();
    // timer until a piece drops via gravity
    double time;
    bool fastFall;
    bool locked;
    void lockPiece() { time = 500; locked = true; }
    // delay to next fall, in ms.
    double dropDelay() const {
        return
            fastFall ? DELAY // fastfall speed
            : pow(0.8-(level-1)*0.007,level-1)*1000; // standard speed
    };
    // fall logic
    void applyGravity();
};


#endif //CSCCLUB_TETRIS_GAME_H
