#pragma once

#include <string>

#include "constants.h"
#include "complex.h"

double mandelbrot(Complex c);

std::string coordinatesToStringKey(int x, int y);