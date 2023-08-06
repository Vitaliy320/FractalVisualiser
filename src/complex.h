#pragma once

// #include <ostream>
#include <iostream>
#include <cmath>

class Complex {
public:
    double real;
    double imaginary;

    Complex();
    Complex(double real_, double imaginary_);
    Complex operator+(const Complex& another);
    Complex operator-(const Complex& another);
    friend std::ostream& operator<<(std::ostream& os, const Complex& number);
    double absolute();
    Complex square();
    Complex multiply(Complex z);
    Complex powerN(int power);
};