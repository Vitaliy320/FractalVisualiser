#include "complex.h"
#include <sstream>
#include <iomanip>


Complex::Complex(): real(0.0), imaginary(0.0){}
Complex::Complex(double real_, double imaginary_): real(real_), imaginary(imaginary_){}

Complex Complex::operator+(const Complex& another){
    return Complex(this->real + another.real, this->imaginary + another.imaginary);
}

Complex Complex::operator-(const Complex& another){
    return Complex(this->real - another.real, this->imaginary - another.imaginary);
}

std::ostream& operator<<(std::ostream& os, const Complex& number){
    std::ostringstream realPartStream;
    realPartStream << std::setprecision(2) << number.real;
    std::string realString {realPartStream.str()};

    std::ostringstream imaginaryPartStream;
    imaginaryPartStream << std::setprecision(2) << number.imaginary;
    std::string imaginaryString {imaginaryPartStream.str()};

    os << realString + " + " + imaginaryString + "i";
    
    return os;
}

double Complex::absolute(){
    return std::sqrt(std::pow(this->real, 2) + std::pow(this->imaginary, 2));
}

Complex Complex::square(){
    return Complex(std::pow(this->real, 2) - std::pow(this->imaginary, 2), 2 * this->real * this->imaginary);
}

Complex Complex::multiply(Complex z){
    return Complex(
        this->real * z.real - this->imaginary * z.imaginary,
        this->real * z.imaginary + this->imaginary * z.real
    );
}

Complex Complex::powerN(int power){
    if (power == 0){
        return Complex(0, 0);
    }

    if (power == 1){
        return *this;
    }

    for (int i = 1; i < power; i++){
        *this = this->multiply(*this);
    }
    return *this;
}