//
// Created by kali on 3/29/22.
//

#ifndef CSCCLUB_TETRIS_UTILITIES_H
#define CSCCLUB_TETRIS_UTILITIES_H


/** wrapper for color operations **/
struct RGB{
    int r, g, b;

    RGB();
    RGB(int r, int g, int b);

    // careful, this is called implicitly!
    RGB(int hex);

    bool operator==(RGB other) const{ return hex() == other.hex(); }

    int hex() const;
};


#endif //CSCCLUB_TETRIS_UTILITIES_H
