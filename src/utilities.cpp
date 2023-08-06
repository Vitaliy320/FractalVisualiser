#include "utilities.h"

double mandelbrot(Complex c){
    Complex zn(0.0, 0.0);
    int n = 0;
    double returnValue;
    while (n < MAX_ITERATIONS){
        n++;
        // zn.imaginary /= std::log10(zn.absolute());
        // zn.imaginary /= std::sin(zn.real);
        zn = (c - zn.square()).powerN(10);
        // zn = zn + c.square();
        // if (zn.absolute() > std::sin(c.absolute())){
        if (zn.absolute() > 2.0){
            if (n == MAX_ITERATIONS){
                return MAX_ITERATIONS;
            }
            return n + 1 - std::log10(std::log2(zn.absolute()));
        }
    }
    return n;
}

std::string coordinatesToStringKey(int x, int y){
    return std::to_string(x) + ":" + std::to_string(y);
}