//
// Created by Zrp200 on 4/11/2022.
//

#ifndef CSCCLUB_TETRIS_BAG_H
#define CSCCLUB_TETRIS_BAG_H

#include <string>
#include "Shape.h"

class Bag{
    static constexpr int SIZE = Shape::T + 1;
    Shape::Piece array[SIZE] = {Shape::O, Shape::I, Shape::J, Shape::L, Shape::Z, Shape::S, Shape::T};
    int i;
    void shuffle();
public:
    Bag(){ shuffle(); }
    Shape::Piece draw(){
        if(i == SIZE) shuffle();
        return array[i++];
    }
};


#endif //CSCCLUB_TETRIS_BAG_H
