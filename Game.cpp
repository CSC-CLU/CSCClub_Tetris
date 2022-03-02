//
// Created by kali on 2/28/22.
//

#include "Game.h"
#include <SDL2/SDL.h>

Game::Game()
{
    _window = nullptr;
    _screenWidth = 1080;
    _screenHeight = 960;
}

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

    _window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
}