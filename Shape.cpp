//
// Created by kali on 2/28/22.
//

#include <random>
#include "Shape.h"

Shape::Shape()
{
    std::default_random_engine random;
    std::uniform_int_distribution<int> distribution(SQUARE,T);
    auto type = static_cast<Piece>(distribution(random));
    switch(type)
    {
        // ／(^ㅅ^)＼ Square
        case SQUARE:
        {
            RGB color(255, 255, 0);
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
            RGB color(255, 255, 255);
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
            RGB color(255, 255, 0);
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
            RGB color(255, 255, 0);
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
            RGB color(255, 255, 0);
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
            RGB color(255, 255, 0);
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