//
// Created by kali on 2/28/22.
//

#include <random>
#include <chrono>
#include "Shape.h"

constexpr static Shape::Square ROTATIONS[Shape::Piece::SIZE/*Piece*/][/*Rotation*/4][Shape::N_SQUARES] = {
    {
        {
            {0,  0},
            {1,  0},
            {0, 1},
            {1, 1}
        }
    },
    {
        {
            {-2, 0},
            {-1, 0},
            {0, 0},
            {1, 0},
        },
        {
            {0, -2},
            {0, -1},
            {0, 0},
            {0, 1}
        },
        {
            {-2, -1},
            {-1, -1},
            {0, -1},
            {1, -1}
        },
        {
            {-1, -2},
            {-1, -1},
            {-1, 0},
            {-1, 1}
        },
    }
};

Shape::Piece randomPiece(){
    // stack overflow said to use this.
    auto random(std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(Shape::Piece::O, Shape::Piece::T);
    return static_cast<Shape::Piece>(distribution(random));
}

Shape::Shape()
    : Shape(randomPiece()){}

// COLOR PATTERN
// 001 O
// 010 I
// 011 J
// 100 L
// 101 Z
// 110 S
// 111 Ts

Shape::Shape(Piece type)
    : piece(type){
    // define either a set number of rotations that are manually defined, or the initial shape
    switch(type){
        // ／(^ㅅ^)＼ Square
        case O:{
            color = 0xFFFF00;
            nRotations = 1;
            break;
        }
            // ／(^ㅅ^)＼ Line
        case I:{
            color = 0xFF7B31;
            nRotations = 4;
            break;
        }
            // ／(^ㅅ^)＼ J
        case J:{
            color = 0x00EEFF;
            shape = new Square[4]{
                {0, 0},
                {1, 0},
                {2, 0},
                {2, 1}
            };
            break;
        }
            // ／(^ㅅ^)＼ L
        case L:{
            color = 0xFF0000;
            shape = new Square[4]{
                {0, 0},
                {1, 0},
                {2, 0},
                {0, 1}
            };
            break;
        }
            // ／(^ㅅ^)＼ Z
        case Z:{
            color = 0xFFA508;
            shape = new Square[4]{
                {-1, -1},
                {0,  -1},
                {0,  0},
                {1,  0},
            };
            break;
        }
            // ／(^ㅅ^)＼ S
        case S:{
            color = 0xFF00FF;
            shape = new Square[4]{
                {-1, 0},
                {0,  0},
                {0,  -1},
                {1,  -1},
            };
            break;
        }
            // ／(^ㅅ^)＼ T
        case T:{
            color = 0x69FF69;
            shape = new Square[4]{
                {-1, 0},
                {0,  0},
                {0,  -1},
                {1,  0},
            };
            break;
        }
    }
    if(nRotations > 0){
        shape = new Square[4];
        for(int r = 0; r < nRotations; r++){
            for(int s = 0; s < N_SQUARES; s++){
                rotations[r][s] = ROTATIONS[type][r][s];
                if(r == 0) shape[s] = rotations[r][s];
            }
        }
    }
}

Shape::Square Shape::getStartingPos() const{
    Square min = {4, 4};
    for(int i = 0; i < Shape::N_SQUARES; i++){
        Shape::Square s = shape[i];
        if(s.x < min.x) min.x = s.x;
        if(s.y < min.y) min.y = s.y;
    }
    min.x *= -1;
    min.y *= -1;
    return min;
}

bool Shape::move(int dx, int dy){
    x += dx;
    y += dy;
    if(isInvalidState()){
        x -= dx;
        y -= dy;
        return false;
    }
    return true;
}

bool Shape::rotate(int _x, int _y, bool force){
    if(nRotations == 0){
        for(int i = 0; i < N_SQUARES; i++){
            shape[i] = {
                _x * shape[i].y % N_SQUARES,
                _y * shape[i].x % N_SQUARES
            };
        }
    }
    else{
        nRotation += _x + nRotations;
        nRotation %= nRotations;
        auto rm = rotations[nRotation];
        for(int i = 0; i < N_SQUARES; i++){
            shape[i] = rm[i];
        }
    }
    if(!force && isInvalidState()){
        // note that if the initial state was invalid this will crash the game (exit code 11)
        rotate(-_x, -_y, true);
        return false;
    }
    return true;
}