//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_GAME_H
#define CSCCLUB_TETRIS_GAME_H
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include<cmath>
#include "Shape.h"
#include "Utilities.h"

enum class GameState
{
    PLAY, EXIT
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
    SDL_Renderer* renderer;

// grid logic (game.cpp)
public:
    constexpr static int ROWS=20, COLS=10; // actual board dimensions
    constexpr static int PADDING=1; // minimum space between elements
    // size of the preview area
    constexpr static int PREVIEW_SIZE = Shape::N_SQUARES;
    // total amount of empty space forced to either size of the grid.
    // currently, enough to fit a preview area on either side of the grid.
    constexpr static int PADDING_HORIZ=PREVIEW_SIZE+2*PADDING;

    int gridLeft() const { return tileLength() * PADDING_HORIZ; }
    int gridRight() const { return tileLength() * (COLS + PADDING_HORIZ); }

    int tileLength() const // the size of a square, which is made to be a specific proportion of the screen to ensure everything fits.
    { return (int)fmin(screenHeight/(ROWS+2*PADDING),screenWidth/(COLS+2*PADDING_HORIZ)); }

    // the color of a specific tile on the grid
    RGB grid[COLS][ROWS];
    bool rowComplete(int y) const {
        for(int i = 0; i < COLS; i++)
        {
            if(grid[i][y] == RGB())
            {
                return false;
            }
        }
        return true;
    }

    bool moveRows(int y)
    {
        for(int i = y; i > 0; i--)
        {
            for(int j = 0; j < COLS; j++)
            {
                grid[j][i] = grid[j][i - 1];
            }
        }
        return clearRow(0);
    }

    bool clearRow(int y)
    {
        for(int i = 0; i < COLS; i++)
        {
            grid[i][y] = RGB();
        }
        if(--toNextLevel == 0) incLevel();
        return true;
    }

private:
    // shape logic
    Shape *nxtShape=new Shape(),*curShape = nullptr;
    void loadNewShape();
    void placeShape();
    bool moveCurShapeDown();
    // FIXME perhaps this sort of logic should go into Display? Would require a rather deep refactor though.
    // render methods (scene.cpp)
    void drawShape(const Shape&);
    void drawSquare(int x,int y);
    void drawSquare(int x,int y,int,int,int);
    void drawSquare(int x, int y, RGB color)
    { drawSquare(x,y,color.r,color.g,color.b); }
    void prepareScene(RGB={255/3,255/3,255/3});
    void presentScene();
    // control logic (game.cpp)
    constexpr static uint32_t DELAY = 16;
    void initSystems();
    void processInput();
    void gameLoop();
    // timer logic
    int level = 1;
    static constexpr int8_t LEVEL_CLEAR = 10;
    int toNextLevel = LEVEL_CLEAR;
    void incLevel();
    int timeLeft = 0;
    bool fastFall = false;
    bool locked = false;
    void lockPiece() { timeLeft = 500/DELAY; locked = true; }
    // dropDelay is in terms of DELAY increments.
    int dropDelay() const {
        return pow(0.8-(level-1)*0.007,level-1)*1000/DELAY;
    };
};


#endif //CSCCLUB_TETRIS_GAME_H
