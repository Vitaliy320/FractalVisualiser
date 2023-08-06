#include "pixel.h"

Pixel::Pixel(){}

Pixel::Pixel(double realPart, double imaginaryPart, RGB rgb){
    _complexPlanePoint = std::make_pair(realPart, imaginaryPart);
    _rgb = rgb;
}

std::pair<double, double> Pixel::getComplexPlanePoint(){
    return _complexPlanePoint;
}

RGB Pixel::getRGB(){
    return _rgb;
}

void Pixel::setComplexPlanePoint(double realPart, double imaginaryPart){
    _complexPlanePoint = std::make_pair(realPart, imaginaryPart);
}

void Pixel::setRGB(RGB rgb){
    _rgb = rgb;
}