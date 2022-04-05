//
// Created by kali on 2/28/22.
//

#include <random>
#include <chrono>
#include "Shape.h"

constexpr static int
squareRotations[1][4][2] = {
        {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}
        }
},
lineRotations[4][4][2] = {
        {
            {-2,0},
            {-1,0},
            {0,0},
            {1,0},
        },
        {
            {0, -2},
            {0,-1},
            {0,0},
            {0,1}
        },
        {
            {-2, -1},
            {-1,-1},
            {0,-1},
            {1,-1}
        },
        {
            {-1, -2},
            {-1,-1},
            {-1,0},
            {-1,1}
        },
};

Shape::Piece randomPiece()
{
    // stack overflow said to use this.
    auto random(std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(Shape::Piece::SQUARE,Shape::Piece::T);
    return static_cast<Shape::Piece>(distribution(random));
}
Shape::Shape()
: Shape( randomPiece() )
{}

Shape::Square Shape::getStartingPos() const {
    Square min = {4,4};
    for(int i=0; i<Shape::N_SQUARES;i++) {
        Shape::Square s = shape[i];
        if(s.x < min.x) min.x = s.x;
        if(s.y < min.y) min.y = s.y;
    }
    min.x *= -1;
    min.y *= -1;
    return min;
}

// COLOR PATTERN
// 001 SQUARE
// 010 LINE
// 011 J
// 100 L
// 101 Z
// 110 S
// 111 Ts

Shape::Shape(Piece type)
{
    switch(type)
    {
        // ／(^ㅅ^)＼ Square
        case SQUARE:
        {
            color = {0, 0, 255};
            shape = new Square[4] {
                {0, 0},
                {1, 0},
                {0, 1},
                {1, 1},
            };
            nRotations = 1;
            for (int r=0; r < nRotations; r++) for (int s=0; s < N_SQUARES; s++) for (int c=0; c < 2; c++)
                rotations[r][s][c] = squareRotations[r][s][c];
            break;
        }
        // ／(^ㅅ^)＼ Line
        case LINE:
        {
            color = {0, 255, 0};
            shape = new Square[4] {
                {-2,1},
                {-1,1},
                {0,1},
                {1,1},
            };
            nRotations = 4;
            for(int r = 0; r < nRotations; r++) for(int s = 0; s < N_SQUARES; s++) for(int c = 0; c < 2; c++)
                rotations[r][s][c] = lineRotations[r][s][c];
            break;
        }
        // ／(^ㅅ^)＼ J
        case J:
        {
            color = {0, 255, 255};
            shape = new Square[4] {
                {0, 0},
                {1, 0},
                {2, 0},
                {2, 1}
            };
            break;
        }
        // ／(^ㅅ^)＼ L
        case L:
        {
            color = {255, 0, 0};
            shape = new Square[4]{
                {0, 0},
                {1, 0},
                {2, 0},
                {0, 1}
            };
            break;
        }
        // ／(^ㅅ^)＼ Z
        case Z:
        {
            color = {255, 0, 255};
            shape = new Square[4] {
                {-1, -1},
                {0, -1},
                {0, 0},
                {1, 0},
            };
            break;
        }
        // ／(^ㅅ^)＼ S
        case S:
        {
            color = {255, 255, 0};
            shape = new Square[4] {
                {-1, 0},
                {0, 0},
                {0, -1},
                {1, -1},
            };
            break;
        }
        // ／(^ㅅ^)＼ T
        case T:
        {
            color = {255, 255, 255};
            shape = new Square[4] {
                {-1, 0},
                {0, 0},
                {0, -1},
                {1, 0},
            };
            break;
        }
    }
}

bool Shape::move(int dx, int dy) {
    x += dx;
    y += dy;
    if ( isInvalidState() ){
        x -= dx;
        y -= dy;
        return false;
    }
    return true;
}

bool Shape::rotate(int _x, int _y) {
    if(nRotations == 0) {
        for(int i=0; i < N_SQUARES; i++) {
            shape[i] = {
                    _x * shape[i].y % N_SQUARES,
                    _y * shape[i].x % N_SQUARES
            };
        }
    } else {
        nRotation += _x + nRotations;
        nRotation %= nRotations;
        auto rm = rotations[nRotation];
        for(int i = 0; i < N_SQUARES; i++)
            shape[i] = rm[i];
    }
    if(isInvalidState()) {
        // note that if the initial state was invalid this will crash the game (exit code 11)
        rotate(-_x,-_y);
        return false;
    }
    return true;
}