//
// Created by kali on 2/28/22.
//

#include <random>
#include <chrono>
#include "Shape.h"

Shape::Shape() {
    // stack overflow said to use this.
    auto random(std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(SQUARE,T);
    *this = Shape(static_cast<Piece>(distribution(random)));
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
            RGB color(0, 0, 255);
            shape = new Square[4] {
                Square(4, 0, color),
                Square(5, 0, color),
                Square(4, 1, color),
                Square(5, 1, color),
            };
            break;
        }
        // ／(^ㅅ^)＼ Line
        case LINE:
        {
            RGB color(0, 255, 0);
            shape = new Square[4] {
                Square(3,0,color),
                Square(4,0,color),
                Square(5,0,color),
                Square(6,0,color),
            };
            break;
        }
        // ／(^ㅅ^)＼ J
        case J:
        {
            RGB color(0, 255, 255);
            shape = new Square[4] {
                Square(4, 0, color),
                Square(5, 0, color),
                Square(6, 0, color),
                Square(6, 1, color)
            };
            break;
        }
        // ／(^ㅅ^)＼ L
        case L:
        {
            RGB color(255, 0, 0);
            shape = new Square[4] {
                Square(4, 0, color),
                Square(5, 0, color),
                Square(6, 0, color),
                Square(4, 1, color),
            };
            break;
        }
        // ／(^ㅅ^)＼ Z
        case Z:
        {
            RGB color(255, 0, 255);
            shape = new Square[4] {
                Square(4, 0, color),
                Square(5, 0, color),
                Square(5, 1, color),
                Square(6, 1, color),
            };
            break;
        }
        // ／(^ㅅ^)＼ S
        case S:
        {
            RGB color(255, 255, 0);
            shape = new Square[4] {
                Square(5, 0, color),
                Square(6, 0, color),
                Square(4, 1, color),
                Square(5, 1, color),
            };
            break;
        }
        // ／(^ㅅ^)＼ T
        case T:
        {
            RGB color(255, 255, 255);
            shape = new Square[4] {
                Square(4, 0, color),
                Square(5, 0, color),
                Square(6, 0, color),
                Square(5, 1, color),
            };
            break;
        }
    }
}