// Author: Caio Rodrigues 
#include <iostream>
#include <vector>
#include <ostream> // Operator: (<<)
#include <string>

enum class Permissions: unsigned {
	executable = 0x01,  // decimal = 1 or (1 << 0)
	writeable  = 0x02,  // decimal = 2 or (1 << 1)
	readable   = 0x04   // decimal = 4 or (1 << 2)
};

Permissions operator | (Permissions lhs, Permissions rhs){
	return static_cast<Permissions>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}
bool operator & (Permissions lhs, Permissions rhs){
	return static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs);
}
// Make permissions enum printable. 
std::ostream& operator<<(std::ostream& os, const Permissions& p){
	os << std::boolalpha; // Make bool printable as 'true' or 'false' instead of 0 or 1
	os << "readable = "   << (p & Permissions::readable)   << "; "
	   << "writeable = "  << (p & Permissions::writeable)  << "; "
	   << "executable = " << (p & Permissions::executable);
	return os;
}
