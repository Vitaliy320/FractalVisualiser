#pragma once

#include <string>
#include <memory>

#include "constants.h"
#include "complex.h"
#include "rgb.h"
#include "hsv.h"
#include "pixel.h"


double mandelbrot(Complex c);

double aPowerZ(Complex z);

double mandelbrotAtIteration(std::shared_ptr<Pixel> pixel);

std::string coordinatesToStringKey(int x, int y);

RGB HSVtoRGB(HSV hsv);