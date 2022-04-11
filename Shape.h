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
        int x=0, y=0;
        constexpr Square() = default;
        constexpr Square(int x, int y): x(x), y(y) {};
    };

    // x and y correspond to the pivot point
    int x=0, y=0;

    Square* shape;
    // shortcut way of accessing shape
    Square operator[](int index) const { return shape[index]; }

    RGB color;

    enum Piece { O, I, J, L, Z, S, T };
    static constexpr int N_SQUARES = 4;

    // necessary offset for the shape to get rendered in the grid
    Square getStartingPos() const;

    Shape(Piece);
    Shape();

    bool rotateR() { return rotate(1, -1); }
    bool rotateL() { return rotate(-1, 1); }

    bool moveDown() { return move(0,1); }
    bool moveR() { return move(1); }
    bool moveL() { return move(-1); }

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
    static bool isInvalidPosition(int x, int y);
    bool isInvalidPosition(Square s) const
    { return isInvalidPosition(s.x+x,s.y+y); }
    bool isInvalidState()
    {
        for(int i=0; i < N_SQUARES; i++)
            if(isInvalidPosition(shape[i])) return true;
        return false;
    }

    bool move(int x,int y=0);
    bool rotate(int x, int y, bool=false);

    int nRotation = 0;
    int nRotations = 0;
    // maximum size for a rotation
    Square rotations[4][4];
};
#endif //CSCCLUB_TETRIS_SHAPE_H
