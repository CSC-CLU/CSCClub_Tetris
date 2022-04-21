//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_GAME_H
#define CSCCLUB_TETRIS_GAME_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <SDL2/SDL_ttf.h>
#include<cmath>
#include "Shape.h"
#include "Utilities.h"
#include "Bag.h"

enum class GameState{
    START, PLAY, GAME_OVER, EXIT
};

class Game{
public:
    Game() : Game(960, 1080){};

    Game(int screenWidth, int screenHeight);

    ~Game();

    void run();

private:
    SDL_Window *window;
    int screenWidth;
    int screenHeight;
    GameState gameState;
    SDL_Renderer *renderer;
    TTF_Font *font;

// grid logic (game.cpp)
public:
    constexpr static int ROWS = 20, COLS = 10; // actual board dimensions
    constexpr static int PADDING = 1; // minimum space between elements
    // size of the preview area
    constexpr static int PREVIEW_SIZE = Shape::N_SQUARES;
    // total amount of empty space forced to either size of the grid.
    // currently, enough to fit a preview area on either side of the grid.
    constexpr static int PADDING_HORIZ = PREVIEW_SIZE + 2 * PADDING;

    int gridLeft() const{ return tileLength() * PADDING_HORIZ; }

    int gridRight() const{ return tileLength() * (COLS + PADDING_HORIZ); }

    int
    tileLength() const // the size of a square, which is made to be a specific proportion of the screen to ensure everything fits.
    { return (int) fmin(screenHeight / (ROWS + 2 * PADDING), screenWidth / (COLS + 2 * PADDING_HORIZ)); }

    // the color of a specific tile on the grid
    RGB grid[COLS][ROWS];

    bool rowComplete(int y) const;

    bool moveRows(int y);

    bool clearRow(int y);

private:
    Bag bag; // for drawing pieces
    // shape logic
    Shape *nxtShape = nullptr, *curShape = nullptr, *heldShape = nullptr;

    void loadNewShape();

    void setCurShape(Shape *);

    void holdShape();

    void placeShape();

    bool moveCurShapeDown();

    // FIXME perhaps this sort of logic should go into Display? Would require a rather deep refactor though.
    // render methods (scene.cpp)
    void drawShape(const Shape &);

    void drawSquare(int x, int y);

    void drawSquare(int x, int y, int, int, int);

    void drawSquare(int x, int y, RGB color){ drawSquare(x, y, color.r, color.g, color.b); }

    void renderPreview(int offset, Shape *, const char *);

    void prepareScene(RGB= {255 / 3, 255 / 3, 255 / 3});

    void presentScene();

    // control logic (game.cpp)
    // time (ms) game waits between renders. Also the value of 1G.
    constexpr static uint32_t DELAY = 16;

    void initSystems();

    void processInput();

    void gameLoop();

    // resets the game to a starting position
    void play();

    // timer logic
    int level;
    static constexpr int8_t LEVEL_CLEAR = 10;
    int toNextLevel;

    void incLevel();

    // timer until a piece drops via gravity
    double time;
    bool fastFall;
    bool locked;

    void lockPiece(){
        time = 500;
        locked = true;
    }

    // delay to next fall, in ms.
    double dropDelay() const{
        return
            fastFall ? DELAY // fastfall speed
                     : pow(0.8 - (level - 1) * 0.007, level - 1) * 1000; // standard speed
    };

    // fall logic
    void applyGravity();
};


#endif //CSCCLUB_TETRIS_GAME_H
