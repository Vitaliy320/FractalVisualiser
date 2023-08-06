#include "rgb.h"

RGB::RGB(){}
RGB::RGB(float red, float green, float blue) : _red(red), _green(green), _blue(blue){}

float RGB::getRed(){
    return _red;
}

float RGB::getGreen(){
    return _green;
}

float RGB::getBlue(){
    return _blue;
}