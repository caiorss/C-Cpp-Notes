// File:   template-metafunction.cpp
// Brief:  Template metaprogramming and generic programming for iterating over containers.
// Author: Caio Rodrigues
//====================================================================================
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <cmath>
#include <functional>
#include <string>

#define disp(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  "\n"

namespace IterUtils{
	template<class Iterator>
	double sumContainer(const Iterator& begin, const Iterator& end){
		double sum = 0.0;
		for(Iterator it = begin; it != end; ++it)
			sum += *it;
		return sum;
	}

	// Sum elements of any type <Container> with methods .begin() and .end()
	// returnign iterators.
	template<class U, class Container>
	auto sumContainer2(const Container& container) -> U{
		U sum{}; // Uniform initialization
		for(auto it = container.begin(); it != container.end(); ++it)
			sum += *it;
		return sum;
	}

	template<class Iterator>
	auto printContainer(
		const Iterator& begin,
		const Iterator& end,
		const std::string& sep = ", " ) -> void
	{
		for(Iterator it = begin; it != end; ++it)
			std::cout <<  *it << sep;
	}

	template<class Container>
	auto printContainer2(
		  const Container& cont
		 ,const std::string& sep = ", "
		) -> void
	{
		// C++11 For-range based loop
		for(const auto& x: cont)
			std::cout <<  x << sep;
	}

	// Higher order function
	// The parameter actions accepts any type which can be called like
	// a function returning void.
	//
	// Note: It doesn't matter as it is possible to use both class T or typename T.
	template<typename Container, typename Function>
	auto for_each (const Container cont, Function action) -> void
	{
		for(const auto& x: cont) action(x);
	}

}; // ----- End of namespace IterUtils ----- //


int main(){
	using std::string;
	// Namespace synonym;
	namespace iu = IterUtils;

	const char nl = '\n';

	const size_t arrsize = 5;
	double carray[arrsize] =  {1.0, 2.0, 4.5, 2.5, 6.0};

	// There are multiple possible ways to initialize a container/aka collection
	auto vec1 = std::vector<double> {1.0, 2.0, 4.5, 2.5, 6.0};
	auto vec2 = std::vector<string> {"c++", "templates", "awesome", "binary"};
	std::list<double>  list1{1.0, 2.0, 4.5, 2.5, 6.0};
	std::list<int>     list2{10, 20, 4, 20, 60};
	std::deque<double> deque1 = {1.0, 2.0, 4.5, 2.5, 6.0};

	std::map<std::string, double> m1 {{"x",      2.3451},
									  {"pi",     3.1415},
									  {"euler", ::exp(1.0)},
									  {"earth-gravity", 9.81}};
	
	std::cout << nl << "=========== Experiment 1 - sumContainer" << nl;
	disp(iu::sumContainer(&carray[0], &carray[0] + arrsize));
	disp(iu::sumContainer(vec1.begin(), vec1.end()));
	disp(iu::sumContainer(list1.begin(), list1.end()));
	disp(iu::sumContainer(deque1.begin(), deque1.end()));

	std::cout << nl << "=========== Experiment 2 - sumContainer2 " << nl;
	disp(iu::sumContainer2<double>(vec1));
	disp(iu::sumContainer2<int>(vec1));
	disp(iu::sumContainer2<double>(list1));
	disp(iu::sumContainer2<int>(list2));

	std::cout << nl << "=========== Experiment 3 - printContainer " << nl;
	std::cout << nl << "Contents of carray  = "; iu::printContainer(&carray[0], &carray[0] + arrsize);
	std::cout << nl << "Contents of vec1  = ";   iu::printContainer(vec1.begin(), vec1.end());
	std::cout << nl << "Contents of vec2  = ";   iu::printContainer(vec2.begin(), vec2.end());
	std::cout << nl << "Contents of list1 = ";   iu::printContainer(list1.begin(), list1.end());

	std::cout << nl << nl << "=========== Experiment 4 - printContainer2 " << nl;
	std::cout << nl << "Contents of vec1  = ";   iu::printContainer2(vec1);
	std::cout << nl << "Contents of vec2  = ";   iu::printContainer2(vec2);
	std::cout << nl << "Contents of list1 = ";   iu::printContainer2(list1);

	std::cout << nl << nl << "=========== Experiment 5 - for_each higher order function " << nl;
	std::cout << nl << "Contents of vec1  = ";
	iu::for_each(vec1, [](double x){ std::cerr << x << ", "; });

	std::cout << nl << "Contents of vec2  = ";   iu::printContainer2(vec2);
	iu::for_each(vec2, [](const std::string& x){ std::cerr << x << ", "; });

	std::cout << nl << "Contents of m1  = " << nl;   
	std::cerr << std::fixed << std::setprecision(3);
	iu::for_each(m1, [](const std::pair<std::string, double>& p){
						 std::cerr << std::setw(15) << p.first << std::setw(10) << p.second << nl;
					 });
	return 0;
}
