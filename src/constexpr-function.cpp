// Author: Caio Rodrigues
// Brief:  Basic demonstration of constexpr function
//-------------------------------------------------------------

#include <iostream>

using BigInt = unsigned long;

// ====== Recursive template metaprogramming (Wikipedia) ====
// Code taken from Wikipedia:
//  + https://en.wikipedia.org/wiki/Template_metaprogramming

template <BigInt N>
struct Factorial 
{
    enum { value = N * Factorial<N - 1>::value };
};

template <>
struct Factorial<0> 
{
    enum { value = 1 };
};


// =================== Constexpr C++11 =========================== // 
// Limitations: No loops, no for-loops, while-loops, no std::cout, std::cerr,
//              no std::sin, std::cos or other math functions computed at runtime. 

// Version A => 
constexpr BigInt factorialA(BigInt n){
	return n == 1 ? 1 : n * factorialA(n - 1);
}

// =================== Constexpr C++14 =========================== //
// Limitations: no std::cout, std::cerr, no std::sin, std::cos or
//              other math functions computed at runtime.


// Version B => Since C++14 (Not possible in C++11)
constexpr BigInt factorialB(BigInt n)
{
	BigInt acc = 1;
	for(size_t i = 1; i <= n; i++)
		acc = acc * i;
	return acc;
}

// Version C => Since C++14 
constexpr auto factorialC(BigInt n) -> BigInt
{
	BigInt acc = 1;
	for(size_t i = 1; i <= n; i++)
		acc = acc * i;
	return acc;
}

void showNumber()

int main()
{
	std::cout << " ======== Old C++ ============= " << "\n";
	
	std::cout << "Factorial(4) = " << Factorial<4>::value << "\n";
	std::cout << "Factorial(5) = " << Factorial<5>::value << "\n";

	static_assert(Factorial<4>::value == 24, "Error: result supposed to be equal to 24");
	static_assert(Factorial<5>::value == 120, "");
	static_assert(Factorial<6>::value == 720, "");

	std::cout << " ======== New C++ >= C++11 ============= " << "\n";

	constexpr BigInt factA4 = factorialA(4);
	constexpr BigInt factA5 = factorialA(5);
	constexpr BigInt factB4 = factorialB(4);
	constexpr BigInt factB5 = factorialB(5);
	
	std::cout << "factorialA(4) = " << factA4 << "\n";
	std::cout << "factorialA(5) = " << factA5 << "\n";
	std::cout << "factorialB(4) = " << factB4 << "\n";
	std::cout << "factorialB(5) = " << factB5 << "\n";

	// Note: factorial(6) will not is not computed at compile-time.
	// Instead, it is be computed at runtime, and the compiler
	// generates an ordinary function named factorialB 
	std::cout << "factorialB(6) = " << factorialB(6) << "\n";
	
	// Generates compilation error when false
	static_assert(factorialA(4) == 24, "");
	static_assert(factorialA(5) == 120, "");
	static_assert(factorialB(4) == 24, "");
	static_assert(factorialB(5) == 120, "");
	static_assert(factorialC(6) == 720, "");
	static_assert(factorialC(7) == 7 * 720, "");	
	
	return 0;
}
