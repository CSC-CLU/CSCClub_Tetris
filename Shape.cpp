//
// Created by kali on 2/28/22.
//

#include <random>
#include "Shape.h"

Shape::Shape()
{
    std::default_random_engine random;
    std::uniform_int_distribution<int> distribution(0,6);
    int type = distribution(random);

    switch(type)
    {
        // ／(^ㅅ^)＼ Square
        case 0:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square *[4];
            shape[0] = new Square(4, 0, color);
            shape[1] = new Square(5, 0, color);
            shape[2] = new Square(4, 1, color);
            shape[3] = new Square(5, 1, color);
            break;
        }
        // ／(^ㅅ^)＼ Line
        case 1:
        {
            auto *color = new Utilities::RGB(255, 255, 255);
            shape = new Square *[4];
            shape[0] = new Square(3, 0, color);
            shape[1] = new Square(4, 0, color);
            shape[2] = new Square(5, 0, color);
            shape[3] = new Square(6, 0, color);
            break;
        }
        // ／(^ㅅ^)＼ J
        case 2:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square*[4];
            shape[0] = new Square(4, 0, color);
            shape[1] = new Square(5, 0, color);
            shape[2] = new Square(6, 0, color);
            shape[3] = new Square(6, 1, color);
            break;
        }
        // ／(^ㅅ^)＼ L
        case 3:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square*[4];
            shape[0] = new Square(4, 0, color);
            shape[1] = new Square(5, 0, color);
            shape[2] = new Square(6, 0, color);
            shape[3] = new Square(4, 1, color);
            break;
        }
        // ／(^ㅅ^)＼ Z
        case 4:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square*[4];
            shape[0] = new Square(4, 0, color);
            shape[1] = new Square(5, 0, color);
            shape[2] = new Square(5, 1, color);
            shape[3] = new Square(6, 1, color);
            break;
        }
        // ／(^ㅅ^)＼ S
        case 5:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square*[4];
            shape[0] = new Square(5, 0, color);
            shape[1] = new Square(6, 0, color);
            shape[2] = new Square(4, 1, color);
            shape[3] = new Square(5, 1, color);
            break;
        }
        // ／(^ㅅ^)＼ T
        case 6:
        {
            auto *color = new Utilities::RGB(255, 255, 0);
            shape = new Square*[4];
            shape[0] = new Square(4, 0, color);
            shape[1] = new Square(5, 0, color);
            shape[2] = new Square(6, 0, color);
            shape[3] = new Square(5, 1, color);
            break;
        }
    }
}