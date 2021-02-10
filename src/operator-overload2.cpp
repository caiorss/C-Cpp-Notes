// File:  operator-overload2.cpp 
// Brief: Arithmetic operator overloading as non-member functions.
// Author: Caio Rodrigues 
//--------------------------------------------

#include <iostream>
#include <cmath>

//---- file Vector2D.h - Declaration: --------------//

/** A struct is just a class with everything public by default. */
struct Vector2D{
	double x, y;
	Vector2D();
	Vector2D(double x, double y);
};

std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs);

/** C++11 Notation */
auto operator- (Vector2D const& lhs, Vector2D const& rhs) -> Vector2D;
auto operator-(const Vector2D& lhs) -> Vector2D;

Vector2D operator*(double k, const Vector2D& rhs);
Vector2D operator*(const Vector2D& rhs, double k);

auto operator/(const Vector2D& lhs, double k) -> Vector2D;


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

Vector2D::Vector2D(): x(0.0), y(0.0)
{
}

Vector2D::Vector2D(double x, double y): x(x), y(y)
{
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec)
{
	return os << "Vector2D{ x = " << vec.x << " ; "
			  << " y = " << vec.y << " } ";
}

Vector2D operator+ (const Vector2D& lhs, const Vector2D& rhs)
{
	return Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

auto operator- (Vector2D const& lhs, Vector2D const& rhs) -> Vector2D
{
	return Vector2D(lhs.x - rhs.x, lhs.y - rhs.y);
}

auto operator-(const Vector2D& lhs) -> Vector2D
{
	return { - lhs.x, -lhs.y };
}

Vector2D operator*(double k, const Vector2D& rhs)
{
	return {k * rhs.x, k * rhs.y};
}

Vector2D operator*(const Vector2D& rhs, double k)
{
	return {k * rhs.x, k * rhs.y};
}

auto operator/(const Vector2D& lhs, double k) -> Vector2D
{
	return {lhs.x / k, lhs.y / k};
}
