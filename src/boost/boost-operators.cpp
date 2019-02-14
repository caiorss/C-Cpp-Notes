// Author: Caio Rodrigues
// Brief:  Demonstration of Boost.Operators library for simplifying operator overloading.
// Boost Operator Docs: https://www.boost.org/doc/libs/1_69_0/libs/utility/operators.htm#ordering
//----------------------------------------------------------------

#include <iostream>
#include <cmath>
#include <cassert>
#include <boost/operators.hpp>

// ============ Pseudo-file: Vec3D.hpp =================

class Vec3D: public boost::less_than_comparable<Vec3D>
		   , public boost::addable<Vec3D, double>
{
private:
	double m_x, m_y, m_z;
public:
	Vec3D(double x, double y, double z);
	double norm() const;

	// Stream insertion operator => Make class printable 
	friend std::ostream& operator<<(std::ostream& os, Vec3D const& vec);	
	
	// Required by: boost::less_than_comparable<Vec3D>
	// From this operator, boost implements the operators (<=), (>), (>=)
	friend bool operator<(Vec3D const& lhs, Vec3D const& rhs);

	// Required by: boost::addable<Vec3D, double>
	// Boost implements: operator+(Vec3D, double) and operator+(double, Vec3D)
	friend Vec3D& operator+=(Vec3D& lhs, double rhs);	
};

// ============ Pseudo-file: main.cpp =================

template<typename variable_t>
void disp(const char* variableName, variable_t const& value)
{
	std::cout << " =>> " << variableName << " = " << value << "\n";
}

int main(){

	Vec3D v1 = {3, 5, 6};
	Vec3D v2 = {12, 5, 9};
	std::cout << std::boolalpha;
	disp("v1", v1); disp("v2", v2);
	disp("v1.norm()", v1.norm());
	disp("v2.norm()", v2.norm());

	std::cout << "\n EXPERIMENT 1 boost::less_than_comparable<Vec3D>" << "\n";
	std::cout << "--------------------------------------------------" << "\n";

	std::cout << "[a] v1 <  v2 = " << (v1 < v2) << "\n";
	std::cout << "[b] v1 <  v2 = " << operator<(v1, v2) << "\n\n";   
	
	std::cout << "[a] v1 <= v2 = " << (v1 <= v2) << "\n";
	std::cout << "[b] v1 <= v2 = " << operator<=(v1, v2) << "\n\n";
	
	std::cout << "[a] v1 >  v2 = " << (v1 > v2) << "\n";
	std::cout << "[b] v1 >  v2 = " << operator>(v1, v2) << "\n\n";
	
	std::cout << "[a] v1 >=  v2 = " << (v1 >= v2) << "\n";
	std::cout << "[b] v1 >=  v2 = " << operator>=(v1, v2) << "\n\n";

	std::cout << "\n EXPERIMENT 2 boost::less_than_comparable<Vec3D>" << "\n";
	std::cout << "--------------------------------------------------" << "\n";
	disp("v1 + 5.0", v1 + 5.0);
	disp("operator+(v1, 5.0)", operator+(v1, 5.0));
	disp("5.0 + v1", 5.0 + v1);
	disp("operator+(5.0, v1", operator+(5.0, v1));
	disp("v1", v1);
	
	return 0;
}

// ============ Pseudo-file: Vec3D.cpp =================

Vec3D::Vec3D(double x, double y, double z)
	: m_x(x), m_y(y), m_z(z)
{
}

double
Vec3D::norm() const 
{
	return std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

std::ostream&
operator<<(std::ostream& os, Vec3D const& vec)
{
	return os << "Vec3D{ "
			  << " x = "   << vec.m_x
			  << " ; y = " << vec.m_y
			  << " ; z = " << vec.m_z
			  << " } ";
}

bool
operator<(Vec3D const& lhs, Vec3D const& rhs)
{
	return lhs.norm() < rhs.norm();
}

Vec3D&
operator+=(Vec3D& lhs, double rhs)
{
	double  d = rhs;
	lhs = {lhs.m_x + d, lhs.m_y + d, lhs.m_z + d};
	return lhs;
}

