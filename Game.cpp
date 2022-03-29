//
// Created by kali on 2/28/22.
//

#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game()
: window(nullptr), screenWidth(960), screenHeight(1080), gameState(GameState::PLAY)
{}

Game::~Game()
{

}

void Game::run()
{
    initSystems();
    gameLoop();
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

void Game::processInput()
{
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt))
    {
        switch(evnt.type)
        {
            case SDL_QUIT:
                gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
                break;
            case SDL_KEYDOWN:
                std::cout << evnt.key.keysym.scancode << std::endl;

        }
    }
}

void Game::gameLoop()
{
    while (gameState != GameState::EXIT)
    {
        processInput();
    }
}