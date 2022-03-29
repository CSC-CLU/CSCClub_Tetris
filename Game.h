//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_GAME_H
#define CSCCLUB_TETRIS_GAME_H
#include <SDL2/SDL.h>
#include <GL/gl.h>

enum class GameState
{
    PLAY, EXIT
};

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    SDL_Window* window;
    int screenWidth;
    int screenHeight;
    GameState gameState;
    SDL_Renderer* renderer;
    void initSystems();
    void processInput();
    void gameLoop();
    // FIXME perhaps this sort of logic should go into Display? Would require a rather deep refactor though.
    // render methods
    void drawSquare(int x,int y);
    void drawSquare(int x,int y,int,int,int);
    // display methods
    void prepareScene();
    void prepareScene(int r, int g, int b);
    void presentScene();
};


#endif //CSCCLUB_TETRIS_GAME_H
