#pragma once

class HSV{
public:
    HSV();
    HSV(float hue, float saturation, float value);
    float getHue();
    float getSaturation();
    float getValue();
private:
    float _hue;
    float _saturation;
    float _value;
};