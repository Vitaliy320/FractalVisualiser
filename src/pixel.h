#pragma once

#include <utility>

#include "rgb.h"
#include "hsv.h"

class Pixel{
public:
    Pixel();
    Pixel(double realPart, double imaginaryPart, RGB rgb);

    std::pair<double, double> getComplexPlanePoint();
    RGB getRGB();

    void setComplexPlanePoint(double realPart, double imaginaryPart);
    void setRGB(RGB rgb);
private:
    std::pair<double, double> _complexPlanePoint;
    RGB _rgb;
};