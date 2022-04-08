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

const Game* gameInstance;
Game::Game(int screenWidth, int screenHeight)
: window(nullptr)
, screenWidth(screenWidth)
, screenHeight(screenHeight)
, gameState(GameState::PLAY)
{
    gameInstance = this;
}

bool Shape::isInvalidPosition(int x,int y)
{
    return x < 0
        || y < 0
        || x >= Game::COLS
        || y >= Game::ROWS
        || !(gameInstance->grid[x][y] == RGB());
}

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

int curPiece = 0;
Shape* cyclePiece(int inc) {
    int N = Shape::T+1;
    curPiece = (curPiece + N + inc) % N;
    return new Shape(static_cast<Shape::Piece>(curPiece));
}

void Game::loadNewShape() {
    delete curShape;
    curShape = nxtShape;
    curShape->setPos(curShape->getStartingPos());
    nxtShape = new Shape();
}

bool Game::moveCurShapeDown() {
    if(curShape->moveDown()) return true;
    placeShape();
    return false;
}

void Game::placeShape() {
    Shape& s = *curShape;
    for(int i=0; i < Shape::N_SQUARES; i++) {
        grid[s[i].x+s.x][s[i].y+s.y] = curShape->color;
    }
    for(int i = 0; i < Shape::N_SQUARES; i++)
    {
        if(rowComplete(s[i].y + s.y))
        {
            moveRows(s[i].y + s.y);
        }
    }
    loadNewShape();
}

bool held = true; // press a button to start the game.
void Game::processInput()
{
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt))
    {
        switch(evnt.type)
        {
            case SDL_WINDOWEVENT:
                switch(evnt.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        screenWidth = evnt.window.data1;
                        screenHeight = evnt.window.data2;
                        break;
                };
                break;
            case SDL_QUIT:
                gameState = GameState::EXIT;
                break;
//            case SDL_MOUSEMOTION:
//                //prepareScene(/*evnt.motion.x * 255 / screenWidth, evnt.motion.y * 255 / screenHeight, 255*/);
//                std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
//                break;
            case SDL_KEYUP:
                if(fastFall && evnt.key.keysym.scancode == SDL_SCANCODE_S) {
                    fastFall = false;
                    std::cout << "fast fall off" << std::endl;
                }
                held = false;
                break;
            case SDL_KEYDOWN:
                if(held) break; else held = true;
                // interact with our piece
                switch (evnt.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                        curShape->y--;
                        break;
                    case SDL_SCANCODE_S:
                        fastFall = true;
                        timeLeft = 0;
                        std::cout << "fast fall on" << std::endl;
                        continue; // already reset timer.
                    case SDL_SCANCODE_A:
                        curShape->moveL();
                        break;
                    case SDL_SCANCODE_D:
                        curShape->moveR();
                        break;
                    case SDL_SCANCODE_Q:
                        curShape->rotateL();
                        break;
                    case SDL_SCANCODE_E:
                        curShape->rotateR();
                        break;
                    case SDL_SCANCODE_EQUALS:
                        std::cout << "level: " << ++level << " --- speed=" << dropDelay() << std::endl;
                        break;
                    case SDL_SCANCODE_MINUS:
                        std::cout << "level: " << --level << " --- speed=" << dropDelay() << std::endl;
                        break;
                    // worth noting this cycles next, so if you want it to start cycling you need to do it twice.
                    case SDL_SCANCODE_RIGHT:
                        loadNewShape();
                        nxtShape = cyclePiece(+1);
                        break;
                    case SDL_SCANCODE_LEFT:
                        loadNewShape();
                        nxtShape = cyclePiece(-1);
                        break;
                    case 40:
                        // move it down all the way
                        while(moveCurShapeDown());
                        break;
                    case SDL_SCANCODE_DELETE:
                        loadNewShape();
                        break;
                }
                //std::cout << evnt.key.keysym.scancode << std::endl;
        }
    }
}

void Game::gameLoop()
{
    loadNewShape();
    while (true)
    {
        prepareScene();
        presentScene();
        SDL_Delay(DELAY);
        processInput();
        if(gameState == GameState::EXIT) break;
        if(fastFall) timeLeft--;
        if(timeLeft-- <= 0) {
            moveCurShapeDown();
            timeLeft = dropDelay();
        }
    }
}