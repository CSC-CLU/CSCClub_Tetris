//
// Created by Zrp200 on 4/11/2022.
//

#include "Bag.h"
#include <random>
#include <chrono>

using namespace std;

void Bag::shuffle(){
    auto random(std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(Shape::Piece::O, Shape::Piece::T);
    // shuffle
    for(auto &piece: array){
        int j = distribution(random);
        Shape::Piece temp = piece;
        piece = array[j];
        array[j] = temp;
    }
    i = 0; // reset to first index
}
