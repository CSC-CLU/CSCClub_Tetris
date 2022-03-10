//
// Created by kali on 2/28/22.
//

#include "Game.h"
#include <SDL2/SDL.h>

Game::Game()
: window(nullptr), screenWidth(960), screenHeight(1080)
{}

Game::~Game()
{

}

void Game::run()
{
    initSystems();
}

void Game::initSystems()
{
    // ／(•ㅅ•)＼ Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
            "Tetris",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screenWidth,
            screenHeight,
            SDL_WINDOW_OPENGL);
}