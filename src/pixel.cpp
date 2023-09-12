#include "pixel.h"

Pixel::Pixel(){}

Pixel::Pixel(int x, int y, double realPart, double imaginaryPart, RGB rgb, int iteration, int maxIterations){
    _x = x;
    _y = y;
    _complexPlanePoint = std::make_pair(realPart, imaginaryPart);
    _zn = Complex(0, 0);
    _rgb = rgb;
    _iteration = iteration;
    _maxIterations = maxIterations;
    _escaped = false;
}

std::pair<int, int> Pixel::getCoordinates(){
    return std::make_pair(_x, _y);
}

std::pair<double, double> Pixel::getComplexPlanePoint(){
    return _complexPlanePoint;
}

Complex Pixel::getComplexPlanePointValue(){
    auto [real, imaginary] = _complexPlanePoint;
    return Complex(real, imaginary);
}

RGB Pixel::getRGB(){
    return _rgb;
}

int Pixel::getIteration(){
    return _iteration;
}

Complex Pixel::getZn(){
    return _zn;
}

Complex* Pixel::getZnPointer(){
    return &_zn;
}

bool Pixel::getEscaped(){
    return _escaped;
}

int Pixel::getX(){
    return _x;
}

int Pixel::getY(){
    return _y;
}

void Pixel::setComplexPlanePoint(double realPart, double imaginaryPart){
    _complexPlanePoint = std::make_pair(realPart, imaginaryPart);
}

void Pixel::setRGB(RGB rgb){
    _rgb = rgb;
}

void Pixel::setIteration(int iteration){
    _iteration = iteration;
}

void Pixel::setZn(Complex zn){
    _zn = zn;
}

void Pixel::setEscapedTrue(){
    _escaped = true;
}

void Pixel::incrementIteration(){
    _iteration++;
}