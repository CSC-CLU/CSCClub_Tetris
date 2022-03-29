//
// Created by kali on 2/28/22.
//

#ifndef CSCCLUB_TETRIS_SHAPE_H
#define CSCCLUB_TETRIS_SHAPE_H

#include "Utilities.h"

// ／(•ㅅ•)＼ Will contain:
//           - 2D array of (x,y) coordinates for each shape
//           - Designation for tile patterns
//           - 2D array of boolean for each square that needs to be checked for each type of collision
//           - int for rotation state

struct Shape
{
    struct Square
    {
        int x;
        int y;
        int h;
        int w;
        RGB color;

        Square(int x, int y, RGB color)
        : x(x), y(y), color(color)
        {};
        Square(int x, int y, int r, int g, int b)
        : x(x),y(y),color(r,g,b)
        {};

    };

    Square* shape;

    enum Piece { SQUARE, LINE, J, L, Z, S, T };
    Shape();
    bool rotateR();
    bool rotateL();
    bool moveDown();
    bool moveR();
    bool moveL();
};


#endif //CSCCLUB_TETRIS_SHAPE_H
