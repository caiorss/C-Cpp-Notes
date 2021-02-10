// Author: Caio Rodrigues
// Brief:  Checks whether type is printable with insertion operator (<<)
// Tags:   template metaprogramming sfinae
//--------------------------------------------------------------------------
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

// std::ostream& operator<<(std::ostream& os, const RHS& rhs);

/**  @brief Checks whether type is printable, or supports the insertion operator (<<).
 *   @details 
 *   This metafunction tests whether type supports the operator (<<) or 
 *   std::ostream& operator<<(std::ostream& os, const RHS& rhs)
 * 
 *   @tparam - Any type, int, char, class, std::string, std::vector ... 
 */
template<typename T>
struct IsPrintable{

	template<typename X, typename = decltype(std::cout << std::declval<X>())>
	static auto check(void*) -> char;

	template<typename X>
	static auto check(...) -> long;  

	static constexpr bool value =
		std::is_same<decltype(check<T>(nullptr)), char>::value; 
};

struct SomeClass{	
};

struct Point{
	int x;
	int y;
	friend auto operator<<(std::ostream& os, Point const& p) -> std::ostream& {
		return os << "Point{ " << p.x << " " << p.y << "} \n";
	}
};

int main(){
	std::cout << std::boolalpha;
	std::cout << "        IsPrintable<int> = " << IsPrintable<int>::value << "\n";
	std::cout << "IsPrintable<std::string> = " << IsPrintable<std::string>::value << "\n";
	std::cout << "  IsPrintable<SomeClass> = " << IsPrintable<SomeClass>::value << "\n";
	std::cout << "      IsPrintable<Point> = " << IsPrintable<Point>::value << "\n";

	return 0;
}
