//
// Created by Zrp200 on 4/7/2022.
//

#include "Game.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <regex>

SDL_Renderer* renderer;
void Game::initScene() {
    // ／(•ㅅ•)＼ Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
            "Tetris",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screenWidth,
            screenHeight,
            SDL_WINDOW_OPENGL);

    if(window == nullptr) {
        throw "SDL Window could not be created";
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // ／(^ㅅ^)＼ Prepare font
    if(TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2 ttf, error: " << TTF_GetError() << std::endl;
    }
    else {
        std::cout << "SDL2 ttf ready to go" << std::endl;
    }

    // ／(^ㅅ^)＼ Font is changed here
    // find current directory
    std::string fontPath = std::regex_replace(
            SDL_GetBasePath(),
            std::regex("cmake-build-debug(/|\\\\)$"),
            "")
                    + "FiraMono-Medium.ttf";
    std::cout << "Font: " << fontPath << std::endl;
    font = TTF_OpenFont(fontPath.c_str(), 22);

    if(font == nullptr){
        std::cout << TTF_GetError() << std::endl;
        exit(1);
    }
}

void Game::presentScene()
{
    SDL_RenderPresent(renderer);
}

void Game::renderPreview(int offset, Shape* shape, SDL_Texture* texture) {
    if(gameState == GameState::PLAY){
        SDL_Rect rect;
        rect.x = gridLeft() + offset * tileLength();
        rect.y = tileLength() * 3;
        rect.w = tileLength() * 4;
        rect.h = tileLength() * 1;

        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    Color previewColor = 0x888888;
    // this generates the prototype preview area.
    for(int i=0; i < PREVIEW_SIZE; i++) {
        // height is half the length (rounded up), as pieces are initially situated horizontally.
        for(int j=0; j < (PREVIEW_SIZE+1)/2; j++) {
            // padding is applied to ensure the preview area doesn't touch the grid
            drawSquare( i+offset, j, previewColor);
        }
    }
    if(shape != nullptr) {
        // offset it to fit inside the preview area.
        shape->setPos(offset);
        drawShape(*shape);
    }
}

void Game::prepareScene(Color backgroundColor)
{
    if(gameState != GameState::PLAY)
        backgroundColor = 0x5b02ce;
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
    SDL_RenderClear(renderer);
    // render grid
    for(int x=0; x < COLS; x++) for(int y=0; y < ROWS; y++) {
            drawSquare(x,y,grid[x][y]);
    }
    // ／(^ㅅ^)＼ Next Piece text
    SDL_Surface *nextSurface = TTF_RenderText_Solid(font, "Next Piece", {255, 255, 255});

    SDL_Texture *nextTexture = SDL_CreateTextureFromSurface(renderer, nextSurface);

    // ／(^ㅅ^)＼ Held Piece text
    SDL_Surface *heldSurface = TTF_RenderText_Solid(font, "Held Piece", {255, 255, 255});

    SDL_Texture *heldTexture = SDL_CreateTextureFromSurface(renderer, heldSurface);

    renderPreview(-PREVIEW_SIZE-PADDING, nxtShape, nextTexture); // next piece on left
    renderPreview(COLS+PADDING, heldShape, heldTexture); // hold piece on right
    if(curShape != nullptr) drawShape(*curShape);

    if(gameState == GameState::START){
        // ／(^ㅅ^)＼ Start game text
        SDL_Surface *startSurface = TTF_RenderText_Solid(font, "Press Start Game to Play", {255, 255, 255});

        SDL_Texture *startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);

        SDL_Rect rect;
        rect.x = 600;
        rect.y = tileLength() * 10;
        rect.w = tileLength() * 20;
        rect.h = tileLength() * 3;

        SDL_RenderCopy(renderer, startTexture, NULL, &rect);
    }
    else if(gameState == GameState::GAME_OVER){
        // ／(^ㅅ^)＼ End game text
        SDL_Surface *endSurface = TTF_RenderText_Solid(font, "Press Start Game to Restart", {255, 255, 255});

        SDL_Texture *endTexture = SDL_CreateTextureFromSurface(renderer, endSurface);

        SDL_Rect rect;
        rect.x = 600;
        rect.y = tileLength() * 20;
        rect.w = tileLength() * 20;
        rect.h = tileLength() * 3;

        SDL_RenderCopy(renderer, endTexture, NULL, &rect);

        // ／(^ㅅ^)＼ Score Title text
        SDL_Surface *scoreTitleSurface = TTF_RenderText_Solid(font, "High Scores", {255, 255, 255});

        SDL_Texture *scoreTitleTexture = SDL_CreateTextureFromSurface(renderer, scoreTitleSurface);

        rect.x = 700;
        rect.y = tileLength() * 3;
        rect.w = tileLength() * 10;
        rect.h = tileLength() * 3;

        SDL_RenderCopy(renderer, scoreTitleTexture, NULL, &rect);
    }

}

// x and y correspond to grid tiles.
// fixme this sets the default color to black.
void Game::drawSquare(int x, int y) {
    SDL_Rect rect;
    rect.w = rect.h = tileLength();
    rect.x = gridLeft() + rect.w * x;
    rect.y = rect.h * (y+PADDING); // need to take into account padding since grid is rendered at the top
    SDL_RenderFillRect(renderer, &rect);
    // this is startlingly inefficient.
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderDrawRect(renderer,&rect);
}
void Game::drawSquare(int x, int y, int r, int g, int b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 0);
    drawSquare(x,y);
}

void Game::drawShape(const Shape& shape) {
    for(int i=0; i < Shape::N_SQUARES; i++) {
        drawSquare(shape.x+shape[i].x,shape.y+shape[i].y, shape.color);
    }
}