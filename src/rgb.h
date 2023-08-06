#pragma once

class RGB{
public:
    RGB();
    RGB(float red, float green, float blue);
    float getRed();
    float getGreen();
    float getBlue();
private:
    float _red;
    float _green;
    float _blue;
};