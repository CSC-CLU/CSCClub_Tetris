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
class Shape
{
public:
    class Square
    {
    public:
        int x;
        int y;
        int h;
        int w;
        Utilities::RGB rgb;

        Square();
        Square(int x, int y, int r, int g, int b);
        Square(int x, int y, Utilities::RGB* rgb);
    };

    Square** shape;

    Shape();
    bool rotateR();
    bool rotateL();
    bool moveDown();
    bool moveR();
    bool moveL();
};


#endif //CSCCLUB_TETRIS_SHAPE_H
