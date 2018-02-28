#pragma once

#include <sstream>

class Complex {
public:
	explicit Complex(double real, double imaginary = .0) :
		_real(real), 
		_imaginary(imaginary) 
	{}

	const Complex& operator+=(const Complex& other) 
	{
		_real = _real + other._real;
		_imaginary = _imaginary + other._imaginary;
		return *this;
	}

	Complex& operator++() 
	{
		++_real;
		return *this;
	}

	const Complex operator++( int ) 
	{
		Complex temp = *this;
		++_real;
		return temp;
	}

	double real() const { return _real; }
	double imaginary() const { return _imaginary; }

	friend const Complex operator+(const Complex& lhs, const Complex& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Complex&); 

private:
	double _real;
	double _imaginary;
};

inline std::ostream& operator<<(std::ostream& os, const Complex& c) 
{
	return os << "(" << c._real << "," << c._imaginary << ")";
}

const Complex operator+(const Complex& lhs, const Complex& rhs)
{
	return Complex(lhs._real + rhs._real, lhs._imaginary + rhs._imaginary);
}
