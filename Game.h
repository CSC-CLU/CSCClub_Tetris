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

// todo y'know there's only one of these. we could have a reference to the one game object, which would let us diversify a lot more.
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

    // grid
    constexpr static int ROWS=20, COLS=10, // actual board dimensions
        L_COLS=6, R_COLS=14, // for guaranteeing space exists.
        T_COLS=COLS+L_COLS+R_COLS;
    int gridLeft() { return tileLength() * L_COLS; }
    int gridRight() { return tileLength() * (COLS + L_COLS); }
    int tileLength() const
    { return (int)fmin(screenHeight/ROWS,screenWidth/T_COLS); }

    Shape *nxtShape=new Shape(),*curShape = nullptr;
    void loadNewShape();

    // actually rotated 90 degrees :/
    RGB grid[COLS][ROWS];
    void placeShape();

    void initSystems();
    void processInput();
    void gameLoop();
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
};


#endif //CSCCLUB_TETRIS_GAME_H
