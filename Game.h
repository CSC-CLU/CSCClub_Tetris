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

public:
    // grid logic
    constexpr static int ROWS=20, COLS=10, // actual board dimensions
        L_COLS=6, R_COLS=14, // for guaranteeing space exists.
        T_COLS=COLS+L_COLS+R_COLS;

    int gridLeft() const { return tileLength() * L_COLS; }
    int gridRight() const { return tileLength() * (COLS + L_COLS); }
    int tileLength() const
    { return (int)fmin(screenHeight/ROWS,screenWidth/T_COLS); }

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

    RGB grid[COLS][ROWS];

private:
    // shape logic
    Shape *nxtShape=new Shape(),*curShape = nullptr;
    void loadNewShape();
    void placeShape();
    // FIXME perhaps this sort of logic should go into Display? Would require a rather deep refactor though.
    // render methods
    void drawShape(const Shape&);
    void drawSquare(int x,int y);
    void drawSquare(int x,int y,int,int,int);
    void drawSquare(int x, int y, RGB color)
    { drawSquare(x,y,color.r,color.g,color.b); }
    // display methods
    void prepareScene();
    void prepareScene(int r, int g, int b);
    void presentScene();
    // setup logic
    void initSystems();
    void processInput();
    void gameLoop();
};


#endif //CSCCLUB_TETRIS_GAME_H
