#pragma once

#include <tuple>

class RGB{
public:
    RGB();
    RGB(float red, float green, float blue);
    float getRed();
    float getGreen();
    float getBlue();
    std::tuple<float, float, float> getRGBTuple();
private:
    float _red;
    float _green;
    float _blue;
};