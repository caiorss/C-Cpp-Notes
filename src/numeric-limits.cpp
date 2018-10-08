/*************************************************************************
 * File:   numeric-limits.cpp 
 * Brief:  Shows the numeric limits for all possible numerical types.
 * Author: Caio Rodrigues
 *************************************************************************/

#include <iostream>
#include <limits>    // Numeric limits 
#include <iomanip>   // setw, and other IO manipulators 
#include <string>    // std::string 
#include <cstdint>   // uint8_t, int8_t, ...
#include <functional>

struct RowPrinter{
	int m_left;  // Left alignment 
	int m_right; // Right alignment  
	RowPrinter(int left, int right): m_left(left), m_right(right){
		// Print bool as 'true' or 'false' instead of 0 or 1.
		std::cout << std::boolalpha;
	}
	
	template<class A>
	auto printRow(const std::string& label, const A& value) const -> void {
		std::cout << std::setw(m_left)  << label
				  << std::setw(m_right) << value << "\n";
	}
};

#define SHOW_INTEGER_LIMITS(numtype) showNumericLimits<numtype>(#numtype)
#define SHOW_FLOAT_LIMITS(numtype)   showFloatPointLimits<numtype>(#numtype)

template <class T>
void showNumericLimits(const std::string& name){
	RowPrinter rp{30, 25};	
	std::cout << "Numeric limits for type: " << name << "\n";
	std::cout << std::string(60, '-') << "\n";
	rp.printRow("Type:",                    name);
	rp.printRow("Is integer:",              std::numeric_limits<T>::is_integer);
	rp.printRow("Is signed:",               std::numeric_limits<T>::is_signed);
	rp.printRow("Number of digits 10:",     std::numeric_limits<T>::digits10);
	rp.printRow("Max Number of digits 10:", std::numeric_limits<T>::max_digits10);

	// RTTI - Run-Time Type Information 
	if(typeid(T) == typeid(uint8_t)
	   || typeid(T) == typeid(int8_t)
	   || typeid(T) == typeid(bool)
	   || typeid(T) == typeid(char)
	   || typeid(T) == typeid(unsigned char)
		){
		// Min Abs - samllest positive value for float point numbers 
		rp.printRow("Min Abs:",         static_cast<int>(std::numeric_limits<T>::min()));
		// Smallest value (can be negative)
		rp.printRow("Min:",             static_cast<int>(std::numeric_limits<T>::lowest()));
		// Largest value  
		rp.printRow("Max:",             static_cast<int>(std::numeric_limits<T>::max()));	
	} else {
		rp.printRow("Min Abs:",         std::numeric_limits<T>::min());
		rp.printRow("Min:",             std::numeric_limits<T>::lowest());
		rp.printRow("Max:",              std::numeric_limits<T>::max());
	}
	rp.printRow("Size in bytes:",       sizeof(T));
	rp.printRow("Size in bits:",        8 * sizeof(T));
	std::cout << "\n";
}

template<class T>
void showFloatPointLimits(const std::string& name){
	RowPrinter rp{30, 25};	
	showNumericLimits<T>(name);
	rp.printRow("Epsilon:",        std::numeric_limits<T>::epsilon());
	rp.printRow("Min exponent:",   std::numeric_limits<T>::min_exponent10);
	rp.printRow("Max exponent:",   std::numeric_limits<T>::max_exponent10);
}

int main(){
	SHOW_INTEGER_LIMITS(bool);
	SHOW_INTEGER_LIMITS(char);
	SHOW_INTEGER_LIMITS(unsigned char);
	SHOW_INTEGER_LIMITS(wchar_t);

	// Standard integers in <cstdint>
	SHOW_INTEGER_LIMITS(int8_t);
	SHOW_INTEGER_LIMITS(uint8_t);
	SHOW_INTEGER_LIMITS(int16_t);
	SHOW_INTEGER_LIMITS(uint16_t);
	SHOW_INTEGER_LIMITS(int32_t);
	SHOW_INTEGER_LIMITS(uint32_t);
	SHOW_INTEGER_LIMITS(int64_t);
	SHOW_INTEGER_LIMITS(uint64_t);

	SHOW_INTEGER_LIMITS(short);
	SHOW_INTEGER_LIMITS(unsigned short);
	SHOW_INTEGER_LIMITS(int);
	SHOW_INTEGER_LIMITS(unsigned int);
	SHOW_INTEGER_LIMITS(long);
	SHOW_INTEGER_LIMITS(unsigned long);
	SHOW_INTEGER_LIMITS(long long);
	SHOW_INTEGER_LIMITS(unsigned long long);
	
	SHOW_FLOAT_LIMITS(float);
	SHOW_FLOAT_LIMITS(double);
	SHOW_FLOAT_LIMITS(long double);
	
    return 0;
}
