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
#include "Bag.h"

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
        return true;
    }

private:
    Bag bag; // for drawing pieces
    // shape logic
    Shape *nxtShape=new Shape(bag.draw()),*curShape = nullptr;
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
    void initSystems();
    void processInput();
    void gameLoop();
};


#endif //CSCCLUB_TETRIS_GAME_H
