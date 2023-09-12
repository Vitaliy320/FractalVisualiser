#pragma once

#include <utility>

#include "rgb.h"
#include "hsv.h"
#include "complex.h"

class Pixel{
public:
    Pixel();
    Pixel(int x, int y, double realPart, double imaginaryPart, RGB rgb, int iteration, int maxIterations);

    std::pair<int, int> getCoordinates();
    std::pair<double, double> getComplexPlanePoint();
    Complex getComplexPlanePointValue();
    RGB getRGB();
    int getIteration();
    Complex getZn();
    Complex* getZnPointer();
    bool getEscaped();
    int getX();
    int getY();

    void setComplexPlanePoint(double realPart, double imaginaryPart);
    void setRGB(RGB rgb);
    void setIteration(int iteration);
    void setZn(Complex zn);
    void setEscapedTrue();

    void incrementIteration();

private:
    int _x;
    int _y;
    std::pair<double, double> _complexPlanePoint;
    RGB _rgb;
    int _iteration;
    int _maxIterations;
    Complex _zn;
    bool _escaped;
};