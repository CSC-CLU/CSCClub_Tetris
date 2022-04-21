//
// Created by kali on 2/28/22.
//

#include "Shape.h"
#ifndef CSCCLUB_TETRIS_BOARD_H
#define CSCCLUB_TETRIS_BOARD_H

// ／(•ㅅ•)＼ Will contain:
//           - 2D array to hold whether a square is occupied or not (and what the tile is)
class Board
{
public:
    bool occupied[10][20];
    int colors[3][10][20];

    bool checkBounds(int x, int y);
    bool checkOccupied(int x, int y);
    bool addShape(Shape s);
};


#endif //CSCCLUB_TETRIS_BOARD_H
