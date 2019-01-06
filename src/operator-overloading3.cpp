// File:  operator-overload3.cpp 
// Brief: Arithmetic operator overloading as friend functions.
// Author: Caio Rodrigues 
//--------------------------------------------

#include <iostream>
#include <cmath>

//---- file Vector2D.h - Declaration: --------------//

class Vector2D{
private:
	double m_x, m_y;
public:
	Vector2D();
	Vector2D(double x, double y);
	double X() const;
	double Y() const;	
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
	friend Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs);

	/** C++11 Notation */
	friend auto operator- (Vector2D const& lhs, Vector2D const& rhs) -> Vector2D;
	friend auto operator-(const Vector2D& lhs) -> Vector2D;

	friend Vector2D operator*(double k, const Vector2D& rhs);
	friend Vector2D operator*(const Vector2D& rhs, double k);
	friend auto operator/(const Vector2D& lhs, double k) -> Vector2D;
};


//-------- Main functions ------------------//

int main(){
	Vector2D v1{3, 5};
	Vector2D v2{5, 6};

	std::cout << "             v1	 = " << v1 << std::endl;
	std::cout << "             v2	 = " << v2 << std::endl;
	std::cout << "         3 * v2	 = " << 3 * v2 << std::endl;
	std::cout << "3 * v2 + 4 * v1	 = " << 3 * v2 + 4 * v1 << std::endl;
	std::cout << "v2 * 3 + v1 * 4	 = " << 3 * v2 + 4 * v1 << std::endl;
	std::cout << "v2 / 10 + v1 / 100 = " << v1 / 10 + v2 / 100 << std::endl;
	return 0;
}

//------ File: Vector2D.cpp -- Implementations ---------------//

Vector2D::Vector2D(): m_x(0.0), m_y(0.0)
{
}

Vector2D::Vector2D(double x, double y): m_x(x), m_y(y)
{
}

double Vector2D::X() const {
	return this->m_x;
}

double Vector2D::Y() const {
	return this->m_y;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec)
{
	return os << "Vector2D{ x = " << vec.m_x << " ; "
			  << " y = " << vec.m_y << " } ";
}

Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs)
{
	return Vector2D(lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y);
}

auto operator- (Vector2D const& lhs, Vector2D const& rhs) -> Vector2D
{
	return Vector2D(lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y);
}

auto operator-(const Vector2D& lhs) -> Vector2D
{
	return { - lhs.m_x, -lhs.m_y };
}

Vector2D operator*(double k, const Vector2D& rhs)
{
	return {k * rhs.m_x, k * rhs.m_y};
}

Vector2D operator*(const Vector2D& rhs, double k)
{
	return {k * rhs.m_x, k * rhs.m_y};
}

auto operator/(const Vector2D& lhs, double k) -> Vector2D
{
	return {lhs.m_x / k, lhs.m_y / k};
}
