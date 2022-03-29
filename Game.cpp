//
// Created by kali on 2/28/22.
//

#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>

void fatalError(std::string errorString)
{
    std::cout << errorString << std::endl;
    std::cout << "Enter any key to quit...";
    int tmp;
    std::cin >> tmp;
    SDL_Quit();
}

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
    int rendererFlags = SDL_RENDERER_ACCELERATED;
    // ／(•ㅅ•)＼ Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
            "Tetris",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screenWidth,
            screenHeight,
            SDL_WINDOW_OPENGL);

    if(window == nullptr)
    {
        fatalError("SDL Window could not be created");
    }

    this->renderer = SDL_CreateRenderer(window, -1, rendererFlags);

}

void Game::prepareScene()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
    SDL_RenderClear(this->renderer);
}

void Game::prepareScene(int r, int g, int b)
{
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 255);
    SDL_RenderClear(this->renderer);

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    for(int i = 0; i < screenWidth; i += (screenWidth / 16))
    {
        SDL_RenderDrawLine(this->renderer, i, 0, i, screenHeight);
    }
    for(int i = 0; i < screenHeight; i += (screenHeight / 16))
    {
        SDL_RenderDrawLine(this->renderer, 0, i, screenWidth, i);
    }
    drawSquare(3,3,255,255,255);
    drawSquare(3,4);
    drawSquare(3,5);
    drawSquare(2,4,255,0,0);
}

// x and y correspond to grid tiles.
void Game::drawSquare(int x, int y) {
    SDL_Rect rect;
    rect.w = screenWidth / 16;
    rect.h = screenHeight / 16;
    rect.x = rect.w * x;
    rect.y = rect.h * y;
    SDL_RenderDrawRect(this->renderer, &rect);
}
void Game::drawSquare(int x, int y, int r, int g, int b) {
    // fixme this permanently sets the color in question.
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
    drawSquare(x,y);
}

void Game::presentScene()
{
    SDL_RenderPresent(this->renderer);
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
                prepareScene(evnt.motion.x * 255 / 960, evnt.motion.y * 255 / 1080, 255);
                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
                break;
            case SDL_KEYDOWN:
                std::cout << evnt.key.keysym.scancode << std::endl;

        }
    }
}

void Game::gameLoop()
{
    prepareScene();
    while (gameState != GameState::EXIT)
    {
        processInput();
        presentScene();
        SDL_Delay(16);
    }
}