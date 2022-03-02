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
    SDL_Window* _window;
    int _screenWidth;
    int _screenHeight;
};


#endif //CSCCLUB_TETRIS_GAME_H
