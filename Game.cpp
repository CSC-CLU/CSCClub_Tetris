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

void Game::prepareScene(RGB backgroundColor)
{
    SDL_SetRenderDrawColor(this->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
    SDL_RenderClear(this->renderer);
    // render grid
    for(int x=0; x < COLS; x++) for(int y=0; y < ROWS; y++) {
        drawSquare(x,y,grid[x][y]);
    }
    int previewOffset = PREVIEW_SIZE+PADDING;
    RGB previewColor = 0x888888;
    // this generates the prototype preview area.
    for(int i=0; i < PREVIEW_SIZE; i++) {
        // height is half the length (rounded up), as pieces are initially situated horizontally.
        for(int j=0; j < (PREVIEW_SIZE+1)/2; j++) {
            // padding is applied to ensure the preview area doesn't touch the grid
            drawSquare( i-previewOffset, j, previewColor);
        }
    }
    drawShape(*curShape);
    nxtShape->setPos(nxtShape->getStartingPos());
    // offset it to fit inside the preview area.
    nxtShape->x -= previewOffset;
    drawShape(*nxtShape);
}

// x and y correspond to grid tiles.
// fixme this sets the default color to black.
void Game::drawSquare(int x, int y) {
    SDL_Rect rect;
    rect.w = rect.h = tileLength();
    rect.x = gridLeft() + rect.w * x;
    rect.y = rect.h * (y+PADDING); // need to take into account padding since grid is rendered at the top
    SDL_RenderFillRect(this->renderer, &rect);
    // this is startlingly inefficient.
    SDL_SetRenderDrawColor(this->renderer,0,0,0,0);
    SDL_RenderDrawRect(this->renderer,&rect);
}
void Game::drawSquare(int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(this->renderer, r, g, b, 0);
    drawSquare(x,y);
}

void Game::drawShape(const Shape& shape) {
    for(int i=0; i < Shape::N_SQUARES; i++) {
        drawSquare(shape.x+shape[i].x,shape.y+shape[i].y, shape.color);
    }
}

// bugged.
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

void Game::presentScene()
{
    SDL_RenderPresent(this->renderer);
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
                        prepareScene();
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
                        curShape->moveDown();
                        break;
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
                        placeShape();
                        break;
                    case SDL_SCANCODE_DELETE:
                        loadNewShape();
                        break;
                }
                prepareScene();
                std::cout << evnt.key.keysym.scancode << std::endl;

        }
    }
}

void Game::gameLoop()
{
    loadNewShape();
    prepareScene();
    while (gameState != GameState::EXIT)
    {
        processInput();
        presentScene();
        SDL_Delay(16);
    }
}