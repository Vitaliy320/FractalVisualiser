#include "utilities.h"

double mandelbrot(Complex c){
    Complex zn(0.0, 0.0);
    int n = 0;
    double returnValue;
    while (n < MAX_ITERATIONS){
        n++;
        // zn.imaginary /= std::log10(zn.absolute());
        // zn.imaginary /= std::sin(zn.real);
        // zn = (c - zn.square()).powerN(10);
        // if (zn.absolute() > std::sin(c.absolute())){

        zn = zn.square() + c;
        // zn = zn.numberToComplexPower();
        if (zn.absolute() > 2.0){
            if (n == MAX_ITERATIONS){
                return MAX_ITERATIONS;
            }
            return n + 1 - std::log10(std::log2(zn.absolute()));
        }
    }
    return n;
}

double aPowerZ(Complex z){
    // Complex zn(0.0, 0.0);
    int n = 0;
    double returnValue;
    while (n < MAX_ITERATIONS){
        n++;
        z = z.numberToComplexPower(0.00006);
        if (z.absolute() > 2.0){
            if (n == MAX_ITERATIONS){
                return MAX_ITERATIONS;
            }
            return n + 1 - std::log10(std::log2(z.absolute()));
        }
    }
    return n;
}

double mandelbrotAtIteration(std::shared_ptr<Pixel> pixel){
    double returnValue;
    pixel->incrementIteration();
    int iteration { pixel->getIteration() };
    pixel->setZn(pixel->getZn().square() + pixel->getComplexPlanePointValue());

    if (pixel->getZn().absolute() > 2){
        pixel->setEscapedTrue();
        if (iteration >= MAX_ITERATIONS){
            return MAX_ITERATIONS;
        }
        return iteration + 1 - std::log10(std::log2(pixel->getZn().absolute()));
    }
    
    auto testValue = iteration + 1 - std::log10(std::log2(pixel->getZn().absolute()));
    return iteration;

    // if (pixel->getZn().absolute() >= 2 && iteration < MAX_ITERATIONS){
    //     pixel->setEscapedTrue();
    //     returnValue = iteration + 1 - std::log10(std::log2(pixel->getZn().absolute()));
    //     if (returnValue > MAX_ITERATIONS){
    //         return MAX_ITERATIONS;
    //     }
    //     return returnValue;
    // }
    
    // return MAX_ITERATIONS;
}

std::string coordinatesToStringKey(int x, int y){
    return std::to_string(x) + ":" + std::to_string(y);
}

RGB HSVtoRGB(HSV hsv){
    float H {hsv.getHue()};
    float S {hsv.getSaturation()};
    float V {hsv.getValue()};

    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        // std::cout << "The given HSV values are not in valid range" << std::endl;
        // return;
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-std::abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    return RGB((r+m)*255, (g+m)*255, (b+m)*255);
}