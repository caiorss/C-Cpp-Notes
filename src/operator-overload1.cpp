// File:  operator-overload1.cpp 
// Brief: Arithmetic operator overloading 
// Author: Caio Rodrigues 
//--------------------------------------------

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

/**-----------------------------------------
 * Complex.hpp  - Class declaration
 *-----------------------------------------*/

/** Complex number */
class Complex{
private:
	double m_real, m_imag;
public:
	/** Make class printable */
	friend std::ostream& operator<<(std::ostream&, const Complex&);	
	
	/** Constructors */
	Complex();
	Complex(double real, double imag);
	/** Named constructor */
	static Complex fromReal(double real);
	static Complex fromImag(double imag);
	
	double Real() const;
	double Imag() const;

	Complex operator-();
	Complex operator+(const Complex& rhs);
	Complex operator-(const Complex& rhs);
	Complex operator*(const Complex& rhs);
	Complex operator/(const Complex& rhs);

	Complex operator+(double d);
	Complex operator-(double d);
	Complex operator*(double scale);
	Complex operator/(double factor);
};

//------ Overloaded Functions ---------------//

double abs(Complex& cpl){
	double x = cpl.Real();
	double y = cpl.Imag();
	return std::sqrt(x * x + y * y);
}

/**-----------------------------------------
 * Main()
 *-----------------------------------------*/

int main(){
	std::cout << " ==== [EXPERIMENT 1] ==============" << std::endl;
	Complex c1;
	Complex c2{2, 5};
	Complex c3{3, 4};
	std::cout << "c1 = " << c1 << " ; c2 = " << c2 << " ; c3 = " << c3 << std::endl;

	std::cout << " ==== [EXPERIMENT 2] ==============" << std::endl;
	auto r1 = c2 + c3;
	std::cout << "-(c2 + c3) = " << -(c2 + c3) << std::endl;
	std::cout << "c2 + c2 = " << r1 << std::endl;
	std::cout << "r1.Real() = " << r1.Real() << " ; r1.Imag() = " << r1.Imag() << std::endl;
	std::cout << " 4 * (c1 + c2 + c3) = " << (c1 + c2 + c3) * 4 << std::endl;
	std::cout << "(c2 * c3 - c1) = " << (c2 * c3 - c1) << std::endl;

	std::cout << "(c2 / c3) * 25 = " << c2 / c3 * 25 << std::endl;
	
	std::cout << " ==== [EXPERIMENT 3] ==============" << std::endl;

	std::cout << "abs(c2) = " << abs(c2) << std::endl;
	
	
	return 0;
}

/**------------------------------------------------------
 * File: Complex.cpp - Class implementation (definition)
 *------------------------------------------------------*/

std::ostream& operator<<(std::ostream& os, const Complex& rhs){
	return os << "Complex{ " << rhs.m_real << " + " << rhs.m_imag << "j" << " }";
}

Complex::Complex(double real, double imag)
	: m_real(real), m_imag(imag)
{
}

Complex::Complex(): Complex(0.0, 0.0)
{
}


Complex Complex::fromReal(double real)
{
	return Complex(real, 0);
}

Complex Complex::fromImag(double imag)
{
	return Complex(0, imag);
}

double Complex::Real() const {
	return m_real;
}

double Complex::Imag() const {
	return m_imag;
}

Complex Complex::operator-(){
	return {-m_real, -m_imag};
}

Complex Complex::operator+(const Complex& rhs)
{
	const auto& self = *this;
	return {self.m_real + rhs.m_real, self.m_imag + rhs.m_imag };
}

Complex Complex::operator-(const Complex& rhs)
{
	const auto& self = *this;
	// (x1 + j . y1)(x2 + j . y2) = x1.x2 + j( -y1.y2 )
	return Complex{self.m_real - rhs.m_real, self.m_imag - rhs.m_imag };
}

Complex Complex::operator*(const Complex& rhs)
{
	return Complex{
		m_real * rhs.m_real - m_imag * rhs.m_imag,
		m_real * rhs.m_imag + m_imag * rhs.m_real
			};
}

Complex Complex::operator/(const Complex& rhs)
{
	double q = rhs.m_real * rhs.m_real + rhs.m_imag * rhs.m_imag;
	double real = this->m_real * rhs.m_real + this->m_imag * rhs.m_imag;
	double imag = this->m_imag * rhs.m_real - this->m_real * rhs.m_imag;
	return { real / q, imag / q };
}

Complex Complex::operator+(double d){
	return Complex{m_real + d, m_imag + d};
}
Complex Complex::operator-(double d){
	return Complex{m_real - d, m_imag - d};
}
Complex Complex::operator*(double scale){
	return {this->m_real * scale, this->m_imag *  scale};
}
Complex Complex::operator/(double factor){
	return Complex(this->m_real / factor, this->m_imag / factor);
}

