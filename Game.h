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
    void prepareScene();
    void presentScene();
};


#endif //CSCCLUB_TETRIS_GAME_H
