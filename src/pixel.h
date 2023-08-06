#pragma once

#include <utility>

#include "rgb.h"
#include "hsv.h"
#include "complex.h"

class Pixel{
public:
    Pixel();
    Pixel(double realPart, double imaginaryPart, RGB rgb, int iteration, int maxIterations);

    std::pair<double, double> getComplexPlanePoint();
    RGB getRGB();
    int getIteration();
    Complex getZn();

    void setComplexPlanePoint(double realPart, double imaginaryPart);
    void setRGB(RGB rgb);
    void setIteration(int iteration);
    void setZn(Complex zn);

    void incrementIteration();

private:
    std::pair<double, double> _complexPlanePoint;
    RGB _rgb;
    int _iteration;
    int _maxIterations;
    Complex _zn;
};