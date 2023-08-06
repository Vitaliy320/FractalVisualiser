#pragma once

class HSV{
public:
    HSV();
    HSV(int hue, int saturation, int value);
    float getHue();
    float getSaturation();
    float getValue();
private:
    float _hue;
    float _saturation;
    float _value;
};