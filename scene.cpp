//
// Created by Zrp200 on 4/7/2022.
//

#include "Game.h"

void Game::presentScene()
{
    SDL_RenderPresent(this->renderer);
}

void Game::renderPreview(int offset, Shape* shape) {
    RGB previewColor = 0x888888;
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

void Game::prepareScene(RGB backgroundColor)
{
    if(gameState != GameState::PLAY)
        backgroundColor = backgroundColor.hex()/2;
    SDL_SetRenderDrawColor(this->renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
    SDL_RenderClear(this->renderer);
    // render grid
    for(int x=0; x < COLS; x++) for(int y=0; y < ROWS; y++) {
        drawSquare(x,y,grid[x][y]);
    }
    renderPreview(-PREVIEW_SIZE-PADDING, nxtShape); // next piece on left
    renderPreview(COLS+PADDING, heldShape); // hold piece on right
    if(curShape != nullptr) drawShape(*curShape);

    if(TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2 ttf, error: " << TTF_GetError() << std::endl;
    }
    else {
        std::cout << "SDL2 ttf ready to go" << std::endl;
    }

    TTF_Font* font = TTF_OpenFont("./kali/home/Documents/ShareTechMono-Regular.ttf", 22);

    if(font == nullptr){
        std::cout << "Could not load font" << std::endl;
        exit(1);
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Next Piece", {255,255,255});

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);

    SDL_FreeSurface(textSurface);

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 100;
    rect.h = 100;

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->renderer);

    SDL_RenderCopy(this->renderer, textTexture, NULL, &rect);
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
