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
        int x, y;

        Square(int x, int y): x(x), y(y) {};
        Square(int c[2]): Square(c[0], c[1]) {}
    };

    // x and y correspond to the pivot point. probably.
    int x=0, y=0;

    // could be converted to a static array ig.
    Square* shape;
    // shortcut way of accessing squares
    Square operator[](int index) const { return shape[index]; }

    RGB color;

    enum Piece { SQUARE, LINE, J, L, Z, S, T };
    static constexpr int N_SQUARES = 4;

    Square getStartingPos() const;

    Shape(Piece);
    Shape();

    // fixme needs to have checks such that invalid moves are discarded.
    bool rotateR() { return rotate(1, -1); }
    bool rotateL() { return rotate(-1, 1); }
    bool moveDown() { y++; };
    bool moveR() { x++; }
    bool moveL() { x--; }

    void setPos(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void setPos(Square s) { setPos(s.x,s.y); }


    Shape(Square *shape)
            : shape(shape) {};

    // ensures that the values and not the array itself is copied.
    Shape(const Shape& shape)
    : Shape(new Square[4]{shape[0], shape[1], shape[2], shape[3]})
    {};

    ~Shape() {
        delete shape;
    }

private:
    bool rotate(int x, int y);

    int nRotation = 0;
    int nRotations = 0;
    // maximum size for a rotation
    int rotations[4][4][2];
};
#endif //CSCCLUB_TETRIS_SHAPE_H
