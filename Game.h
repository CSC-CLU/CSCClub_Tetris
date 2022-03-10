//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_GAME_H
#define CSCCLUB_TETRIS_GAME_H
#include <SDL2/SDL.h>
#include <GL/gl.h>


class Game
{
public:
    Game();
    ~Game();

    void run();

    void initSystems();

private:
    SDL_Window* window;
    int screenWidth;
    int screenHeight;
};


#endif //CSCCLUB_TETRIS_GAME_H
