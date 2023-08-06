#include "pixel.h"

Pixel::Pixel(){}

Pixel::Pixel(double realPart, double imaginaryPart, RGB rgb, int iteration, int maxIterations){
    _complexPlanePoint = std::make_pair(realPart, imaginaryPart);
    _rgb = rgb;
    _iteration = iteration;
    _maxIterations = maxIterations;
}

std::pair<double, double> Pixel::getComplexPlanePoint(){
    return _complexPlanePoint;
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

void Pixel::incrementIteration(){
    _iteration++;
}