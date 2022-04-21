//
// Created by kali on 3/29/22.
//

#ifndef CSCCLUB_TETRIS_COLOR_H
#define CSCCLUB_TETRIS_COLOR_H

#include<SDL2/SDL_pixels.h>

/** wrapper for color operations **/
struct Color : SDL_Color
{
    // default color
    constexpr Color() : SDL_Color({100,100,100}) {}
    constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a=0)
        : SDL_Color({r,g,b,a}) {}
    constexpr Color(int hex) : Color(
            hex >> 16 & 0xFF,
            hex >> 8 & 0xFF,
            hex & 0xFF){}

    constexpr bool operator==(Color other) const { return hex() == other.hex(); }

    constexpr int hex() const { return r << 16 | g << 8 | b; }
};


#endif //CSCCLUB_TETRIS_COLOR_H
