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

Game::Game(int screenWidth, int screenHeight)
: window(nullptr)
, screenWidth(screenWidth)
, screenHeight(screenHeight)
, gameState(GameState::PLAY)
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
    prepareScene(256/3,256/3,256/3);
}

#include "Shape.h"
int curPiece = 0;
void Game::prepareScene(int r, int g, int b)
{
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 255);
    SDL_RenderClear(this->renderer);
    // render grid
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
    for(int i = gridLeft(); i <= gridRight(); i += tileLength())
    {
        SDL_RenderDrawLine(this->renderer, i, 0, i, tileLength()*ROWS);
    }
    for(int i = 0; i <= ROWS*tileLength(); i += tileLength())
    {
        SDL_RenderDrawLine(this->renderer, gridLeft(), i, gridRight(), i);
    }
    // this generates the prototype preview area.
    for(int i=0; i < L_COLS-2; i++) {
        for(int j=0; j < L_COLS-2; j++) {
            int c = 255/2;
            drawSquare(i-L_COLS+1, j+1,c,c,c);
        }
    }
    Shape testShape(static_cast<Shape::Piece>(curPiece));
    curPiece = (curPiece+1) % (Shape::T+1);
    for(int i=0; i < 4; i++) {
        Shape::Square square = testShape.shape[i];
        drawSquare(-square.x+1, square.y+1, square.color.r, square.color.g, square.color.b);
    }
}

// x and y correspond to grid tiles.
// fixme this sets the default color to black.
void Game::drawSquare(int x, int y) {
    SDL_Rect rect;
    rect.w = rect.h = tileLength();
    rect.x = gridLeft() + rect.w * x;
    rect.y = rect.h * y;
    SDL_RenderFillRect(this->renderer, &rect);
    // this is startlingly inefficient.
    SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
    SDL_RenderDrawRect(this->renderer,&rect);
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
                //prepareScene(/*evnt.motion.x * 255 / screenWidth, evnt.motion.y * 255 / screenHeight, 255*/);
                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
                break;
            case SDL_KEYDOWN:
                prepareScene();
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