#include "hsv.h"

HSV::HSV(){}
HSV::HSV(float hue, float saturation, float value) : _hue(hue), _saturation(saturation), _value(value){}

float HSV::getHue(){
    return _hue;
}

float HSV::getSaturation(){
    return _saturation;
}

float HSV::getValue(){
    return _value;
}