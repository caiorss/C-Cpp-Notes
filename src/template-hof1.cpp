/**  File:     template-hof1.cpp 
  *  Brief:    Shows how to implement template higher order functions which operates on containers.
  *  Features: Template metaprogramming, C++11, functional programming and STL.
  ****************************************************************************/
#include <iostream>
#include <cmath>
#include <list>
#include <deque>
#include <vector>
#include <functional> 
#include <iomanip>

/** Apply a function to every element of a container */
template<class ELEM, class ALLOC, template<class, class> class CONTAINER>
void forRange1(CONTAINER<ELEM, ALLOC>& cont, std::function<void (ELEM&)> fn){
        for(auto i =  std::begin(cont); i != std::end(cont); i++)
                fn(*i);
}

/** Apply a function to every element of a container */
template<class CONTAINER>
void forRange2(CONTAINER& cont, std::function<void (decltype(cont.front()))> fn){
        for(auto i =  std::begin(cont); i != std::end(cont); i++)
                fn(*i);
}

/** Template for folding over a container in a similar way to the higher order function fold. 
 * Note: 
 * + CONTAINER parameter accepts any argument which has .begin() and .end() methods 
 *   returning iterators. 
 * + STEPFN type parameters accepts any function-object, function pointer or lambda 
 *   whith the following signature: (ACC, X) => ACC where ACC is the accumulator type 
 *   and X is the type of the container element. 
 */
template<class CONTAINER, class ACC, class STEPFN>
auto foldRange(CONTAINER& cont, const ACC& init, STEPFN fn) -> ACC {
        ACC acc{init};
        for(auto i =  std::begin(cont); i != std::end(cont); i++)
                acc = fn(*i, acc);
        return acc;
}

int main(){
        std::ios_base::sync_with_stdio(false);

        std::vector<int> vec{1, 2, 400, 100};
        std::list<int>   lst{1, 2, 400, 100};

        // Requires template argument 
        std::cout << "===== EXPERIMENT 1 =================\n";
		std::cout << "forRange1 - Vector" << "\n";
        forRange1<int>(vec, [](int x){ std::cout << std::setw(5) << x << " "; });
        std::cout << "\n";
        std::cout << "forRange1 - List" << "\n";
        forRange1<int>(lst, [](int x){ std::cout << std::setw(5) << x << " "; });
        std::cout << "\n";

        // Doesn't require the template argument as the compiler can infer its type.
        std::cout << "===== EXPERIMENT 2 =================\n";
        std::cout << "forRange1 - Vector" << "\n";
        forRange2(vec, [](int x){ std::cout << std::setw(5) << x << " "; });
        std::cout << "\n";
        std::cout << "forRange1 - list" << "\n";
        forRange2(lst, [](int x){ std::cout << std::setw(5) << x << " "; });
        std::cout << "\n";

        std::cout << "===== EXPERIMENT 3 =================\n";
        int result1 = foldRange(vec, 0, [](int x, int acc){
											return x + acc;
										});	
        std::cout << "sum(vec1) = " << result1 << "\n" ;
        int result2 = foldRange(lst, 0, std::plus<int>());	
        std::cout << "sum(lst) = " << result2 << "\n" ;

        std::cout << "product(lst) = " << foldRange(lst, 1, std::multiplies<int>()) << "\n" ;
        return 0;
}
