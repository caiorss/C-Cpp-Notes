// Author: Caio Rodrigues
// File:   fold-expressions1.cpp
// Brief:  Demonstrations of C++17 Fold Expressions
// - https://caiorss.github.io/C-Cpp-Notes/index.html
//---------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <tuple>
#include <any>

template<typename ... Args>
auto multiply(Args ... args)
{
    return (args * ...);
}

template<typename ... Args>
void printArguments(Args&& ...  args )
{
   ((std::cout << typeid(args).name()  << " " << args << std::endl), ...);
}

template<typename ... Args>
std::vector<std::type_info const*>
GetTypesFromArgs1(){
    std::vector<std::type_info const*> tinfo;
    std::cout << "Received " << sizeof...(Args)
              << " types arguments" << std::endl;
    ((tinfo.push_back(&typeid(Args))), ...) ;
    return tinfo;
}

template<typename ... Args>
std::vector<std::type_info const*>
GetTypesFromArgs2(Args const& ... args){    
    std::cout << "Received " << sizeof...(Args)
              << " types arguments" << std::endl;
	return {&typeid(args) ...};
}

// Experimental: Still does not work, in some compilers, the index is not incremented.
// It still falls in the undefined behavior.
#ifdef EANBLE_EXPERIMENTAL
template<typename R, typename ... Args>
auto TypeErasureFunction(R funptr (Args ...))
	-> std::function<std::any (std::vector<std::any>&)>
{
	return [funptr](std::vector<std::any> xs) -> std::any
		   {
			   int idx = -1;
			   std::any res;
			   res = funptr((idx++, std::any_cast<Args>(xs[idx])) ...);
			   return res;
		   };
}

int someFunction(int x, double w, const char* name)
{
	std::cout << " Call someFunction()"
			  << "    x = " << x
			  << "    w = " << w
			  << " name = " << name
			  << std::endl;
	return 0;
}
#endif 


int main()
{
	std::cout << "\n ======= EXPERIMENT 1 ===============\n";	
	std::cout << " => Result 1 = " << multiply(1, 2, 3, 4, 5, 6) << std::endl;
	std::cout << " => Result 2 = " << multiply(3.5, 1.65, 0.25, 10.98, 100.5, 6) << std::endl;

	std::cout << "\n ======= EXPERIMENT 2 ===============\n";
	printArguments(10, 200.5, "hello world", 'x');

	std::cout << "\n ======= EXPERIMENT 3 ===============\n";
	auto tinfo = GetTypesFromArgs1<int, double, const char*, std::string>();
	for(auto const& t: tinfo)
	{
		std::cout << "Name = " << t->name()
				  << " - id = " << t->hash_code()
				  << std::endl;
	}	

	std::cout << "\n ======= EXPERIMENT 4 ===============\n";
	auto tinfo2 = GetTypesFromArgs2(200, 'x', 9.87, std::string("hello world"), "CPP17");
	for(auto const& t: tinfo2)
	{
		std::cout << "Name = " << t->name()
				  << " - id = " << t->hash_code()
				  << std::endl;
	}

	#ifdef EANBLE_EXPERIMENTAL
	std::cout << "\n ======= EXPERIMENT 5 ===============\n";
	std::vector<std::any> args = {200, 87.5, "'C++17 type erasure'" };
	auto erased_someFunction = TypeErasureFunction(&someFunction);
	erased_someFunction(args);
	#endif
	
	return 0;
}	
	
