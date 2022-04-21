//
// Created by Zrp200 on 4/7/2022.
//

#include "Game.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

void Game::presentScene()
{
    SDL_RenderPresent(this->renderer);
}

void Game::renderPreview(int offset, Shape* shape, const char* label) {
    if(gameState == GameState::PLAY) {
        // ／(^ㅅ^)＼ Next Piece text
        SDL_Surface *textSurface = TTF_RenderText_Solid(this->font, label, {255, 255, 255});

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);

        SDL_FreeSurface(textSurface);

        SDL_Rect rect;
        rect.x = gridLeft() + offset * tileLength();
        rect.y = tileLength() * 3;
        rect.w = tileLength() * 4;
        rect.h = tileLength() * 1;

        SDL_RenderCopy(this->renderer, textTexture, NULL, &rect);
    }

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
    renderPreview(-PREVIEW_SIZE-PADDING, nxtShape, "Next Piece"); // next piece on left
    renderPreview(COLS+PADDING, heldShape, "Held Piece"); // hold piece on right
    if(curShape != nullptr) drawShape(*curShape);

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
