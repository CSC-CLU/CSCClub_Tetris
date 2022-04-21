//
// Created by kali on 3/29/22.
//

#ifndef CSCCLUB_TETRIS_COLOR_H
#define CSCCLUB_TETRIS_COLOR_H

/** wrapper for color operations **/
struct Color
{
    unsigned char r,g,b;

    // default color
    constexpr Color() : Color(100, 100, 100) {}
    constexpr Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b){}
    constexpr Color(int hex) : r(hex >> 16 & 0xFF), g(hex >> 8 & 0xFF), b(hex & 0xFF){}

    constexpr bool operator==(Color other) const { return hex() == other.hex(); }

    constexpr int hex() const { return r << 16 | g << 8 | b; }
};


#endif //CSCCLUB_TETRIS_COLOR_H
