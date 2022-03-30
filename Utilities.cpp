//
// Created by kali on 3/29/22.
//

#include "Utilities.h"

RGB::RGB()
: RGB(255,255,255)
{}

RGB::RGB(int r, int g, int b)
: r(r)
, g(g)
, b(b)
{}

RGB::RGB(int hex)
: r(hex >> 16 & 0xFF)
, g(hex >> 8 & 0xFF)
, b(hex & 0xFF)
{}

int RGB::hex() const {
    return r << 16 | g << 8 | b;
}