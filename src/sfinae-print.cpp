// Author: Caio Rodrigues
// Brief:  Print type if it is printable. Demonstrate SFINAE and C++17 if constexpr code simplification.
// Tags:   template metaprogramming sfinae c++17 c++11 and if constexpr
//--------------------------------------------------------------------------
#include <iostream>
#include <type_traits>

#include <string>
#include <vector>

template<typename T>
struct IsPrintable{
	template<typename X, typename = decltype(std::cout << std::declval<X>())>
	static auto check(void*) -> char;

	template<typename X> 
	static auto check(...) -> long;  

	static constexpr bool value =
		std::is_same<decltype(check<T>(nullptr)), char>::value; 
};

// ==================== Function printIfPrintableA ===================== // 
// Version A: Uses std::enable_if as function-return type 

// Overload selected when type is printable 
template<typename T>
typename std::enable_if<IsPrintable<T>::value, void>::type
printIfPrintableA(const char* text, T const& value)
{
	std::cout << "[printIfPrintableA] "
			  << "Value of object of type <" << text << "> = " << value << "\n";
}

// Overload selected when type is not printable. Or does not have the
// operator (<<) defined for it.
template<typename T>
typename std::enable_if<!IsPrintable<T>::value, void>::type																  
printIfPrintableA(const char* text, T const& value)
{
	std::cout << "[printIfPrintableA] "
			  << "Value of object of type <" << text << "> = " << "[NOT PRINTABLE]" << "\n";
}

// ==================== Function printIfPrintableB ===================== //
// Version B uses: Uses std::enable_if as function-return as trailing-return type
// 

template<typename T>
auto printIfPrintableB(const char* text, T const& value)
	-> typename std::enable_if<IsPrintable<T>::value, void>::type
{
	std::cout << "[printIfPrintableB] "
			  << "Value of object of type <"
			  << text << "> = " << value << "\n";
}

template<typename T>
auto printIfPrintableB(const char* text, T const& value)
	-> typename std::enable_if<!IsPrintable<T>::value, void>::type																  
{
	std::cout << "[printIfPrintableB] "
			  << "Value of object of type <" << text
			  << "> = " << "[NOT PRINTABLE]" << "\n";
}

// ==================== Function printIfPrintableC ===================== // 
// Version C: Uses std::enable_if as template type parameter.

template<typename T,
		 typename std::enable_if<IsPrintable<T>::value, void>::type* = nullptr>
void printIfPrintableC(const char* text, T const& value)
{
	std::cout << "[printIfPrintableC] "
			  << "Value of object of type <"
			  << text << "> = " << value << "\n";
}

template<typename T,
		 typename std::enable_if<!IsPrintable<T>::value, void>::type* = nullptr>
void printIfPrintableC(const char* text, T const& value)
{
	std::cout << "[printIfPrintableC] "
			  << "Value of object of type <"
			  << text << "> = " << "[NOT PRINTABLE]" << "\n";
}

// ==================== Function printIfPrintableD ===================== // 

template<typename T>
void printIfPrintableD(const char* text, T const& value)
{

	std::cout << "[printIfPrintableD] "
			  << "Value of object of type <" << text << "> = ";
	if constexpr (IsPrintable<T>::value)
		std::cout << value << "\n";
	else
		std::cout << "[NOT PRINTABLE]" << "\n";					 
}

// Not printable type 
struct SomeClass{	
};

struct Point{
	int x;
	int y;
	Point(int x, int y): x(x), y(y) { }
	friend auto operator<<(std::ostream& os, Point const& p) -> std::ostream& {
		return os << "Point{ " << p.x << " " << p.y << "} \n";
	}
};

int main(){
	std::cout << std::boolalpha;

	std::cout << "\n======= EXPERIMENT A ======================\n";	
	printIfPrintableA("int", 100);
	printIfPrintableA("double", 20.4);
	printIfPrintableA("SomeClass", SomeClass());
	printIfPrintableA("Point", Point{5, 6});

	std::cout << "\n======= EXPERIMENT B ======================\n";
	printIfPrintableB("int", 100);
	printIfPrintableB("double", 20.4);
	printIfPrintableB("SomeClass", SomeClass());
	printIfPrintableB("Point", Point{15, 20});

	std::cout << "\n======= EXPERIMENT C ======================\n";
	printIfPrintableC("int", 100);
	printIfPrintableC("double", 20.4);
	printIfPrintableC("SomeClass", SomeClass());
	printIfPrintableC("Point", Point{15, 20});

	std::cout << "\n======= EXPERIMENT D (C++17) ===============\n";
	printIfPrintableD("int", 100);
	printIfPrintableD("double", 20.4);
	printIfPrintableD("SomeClass", SomeClass());
	printIfPrintableD("Point", Point{15, 20});	
	
	return 0;
}
