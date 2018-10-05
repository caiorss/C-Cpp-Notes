/* File:        cpp-functor.cpp 
 * Author:      Caio Rodrigues caiorss <dot> rodrigues <at> gmail <dot> com
 * Brief:       Demonstration of C++ functor-function object. 
 * Description: 
 * Note:        This example focus on scientific computing, but can be extended 
 *              to other domains.
 *
 * Compile:     $ clang++ cpp-functor.cpp -o cpp-functor.bin -g -std=c++11 -Wall -Wextra && ./cpp-functor.bin
 *=============================================================================*/

#include <iostream>
#include <iomanip>    // Stream manipulator std::fixed, std::setw ... 
#include <vector>
#include <cmath>      // sin, cos, tan, exp ... M_PI, M_E ...
#include <functional> // std::function 

// C++ interface class idiom => math functor base class
class IMathFunctor {
public:
	// Pure virtual function
	// => const -> Means that the function cannot change the object internal state.
	// => (= 0) -> Means abstract member function or abstract method.
	virtual double operator()(double x)	const = 0;
	virtual ~IMathFunctor() = default;
};

/** Linear Function "C++ functor." - function-object 
 *   LinFun(x) = A * x + B
 */
class LinFun: public IMathFunctor {
private:
	// Linear coefficient or line slope 
	double _a; 
	double _b;
public:
	LinFun(double a, double b): _a(a), _b(b){}
	auto getA()					-> double	{ return _a;}
	auto setA(double a)			-> void		{ _a = a; }
	auto getB()					-> double	{ return _b;}
	auto setB(double b)			-> void		{ _b = b; }
	
	// Function-call operator => Makes this object callable
	//------------------------------
	// double operator()(double x)	-> double	{ return _a * x + _b;}	
	double operator()(double x)	const { return _a * x + _b;}	
};

/** -- Quadratic Function Y(x) = A * X^2 + B * X + C --*/
class QuadFun: public IMathFunctor {
private:
	// Linear coefficient or line slope 
	double _a; 
	double _b;
	double _c;
public:
	QuadFun(){}
	QuadFun(double a, double b, double c): _a(a), _b(b), _c(c){}
	auto getA()					-> double	{ return _a;}
	auto setA(double a)			-> void		{ _a = a; }
	auto getB()					-> double	{ return _b;}
	auto setB(double b)			-> void		{ _b = b; }
	auto getC()					-> double	{ return _c;}
	auto setC(double c)			-> void		{ _c = c; }	
	// Function-call operator 
	auto operator()(double x) const	-> double	{ return _a * x * x + _b * x + _c;}
};

/* Higher order function using dynamic polymorphism (inheritance)
 * Accepts any implementation of IMathFunctor
 */
void tabulateDynamic(const IMathFunctor& fun, double start, double stop, double step){
	std::cout << std::fixed << std::setprecision(3);
	for(double x = start; x <= stop ; x += step)
		std::cout << std::setw(10) << x << std::setw(10) << fun(x) << "\n";
}

/** The advantage of dynamic polymorphism is that instances of polymorphic types
 *  can be stored in containers and addressed in the same way.
 */
void tabulateDynamicList(const std::vector<IMathFunctor*> funlist, double start, double stop, double step){
	std::cout << std::fixed << std::setprecision(3);
	for(double x = start; x <= stop ; x += step){
		std::cout << std::setw(10) << x;
		// const auto& is used for avoid uncessary copies 
		for(const auto& ptr: funlist)
			std::cout << std::setw(10) << ptr->operator()(x);
		std::cout << "\n";
	}
}

void tabulateLambdaList(
	 const std::vector<std::function<double (double)>> funlist
	,double start
	,double stop
	,double step
	){
	std::cout << std::fixed << std::setprecision(3);
	for(double x = start; x <= stop ; x += step){
		std::cout << std::setw(10) << x;
		// const auto& is used for avoid uncessary copies 
		for(const auto& fun: funlist)
			std::cout << std::setw(10) << fun(x);
		std::cout << "\n";
	}
}

/* Higher order function using static polymorphis (templates)
 * The advantage of templates is that they can remove the dynamic polymorphism 
 * (aka subtyping polymorphism overhead) making the code faster as the 
 * the function to be called is resolved at compile-time.
 * 
 * This function accepts any object that can be called, it means, 
 * ordinary function, functors and C++11 lambda functions.
 */
template<class Function>
void tabulateStatic(const Function& fun, double start, double stop, double step){
	std::cout << std::fixed << std::setprecision(3);
	for(double x = start; x <= stop ; x += step)
		std::cout << std::setw(10) << x << std::setw(10) << fun(x) << "\n";
}

double ordinaryFunction(double x){
	return 3 * x;
}

int main(){
	char nl = '\n';

	// Function objects can be used and called like ordinary functions
	// like sin, cos, tan ... 
	// Function linear object - modelling a linear function 3 * x + 4.0 
	LinFun fun1(3.0, 4.0);
	std::cout << "a = " << fun1.getA() << " ; b = " << fun1.getB() << nl;
	std::cout << "fun1(3.0) = " << fun1(3.0) << nl;  
	std::cout << "fun1(4.0) = " << fun1(4.0) << nl;
	// Alternative way to call a "functor"
	std::cout << "fun1(5.0) = " << fun1.operator()(5.0) << nl;

	std::cout << "=======================" << nl;
	
	QuadFun fun2(2.0, 3.0, 4.0);
	std::cout << "a = " << fun2.getA() << " ; b = " << fun2.getB() << " ; c = " << fun2.getC() << nl;
	std::cout << "fun2(3.0) = " << fun2(3.0) << nl;
	std::cout << "fun2(4.0) = " << fun2(4.0) << nl;
	std::cout << "fun2(5.0) = " << fun2(5.0) << nl;

	std::cout << "======= [1] Client Code using dynamic polymorphism  ================" << nl;
	std::cout << " -----> Tabulating fun1" << nl;
	tabulateDynamic(fun1, 0.0, 5.0, 1.0);
	std::cout << " -----> Tabulating fun2" << nl;
	tabulateDynamic(fun2, 0.0, 5.0, 1.0);

	std::cout << "======= [2] Client Code using dynamic polymorphism  ================" << nl;
	tabulateDynamicList({&fun1, &fun2}, 0.0, 5.0, 1.0);

	std::cout << "======= Client Code using static polymorphism (template)  ================" << nl;
	std::cout << " -----> Tabulating fun1" << nl;
	tabulateStatic(fun1, 0.0, 5.0, 1.0);
	std::cout << " -----> Tabulating fun2" << nl;
	tabulateStatic(fun2, 0.0, 5.0, 1.0);

	std::cout << " -----> Tabulating lambda function f(x) = x * x" << nl;
	tabulateStatic([](double x){ return x * x; }, 0.0, 5.0, 1.0);

	std::cout << " -----> Tabulating ordinary function f(x) = 3 * x" << nl;
	tabulateStatic(ordinaryFunction, 0.0, 5.0, 1.0);
	
	std::cout << " -----> Tabulating ordinary function f(x) = exp(x)" << nl;
	tabulateStatic(exp, 0.0, 5.0, 1.0);

	std::cout << "======= Client Code using C++11 lambda std::function  ================" << nl;
	tabulateLambdaList({fun1, fun2, exp, ordinaryFunction}, 0.0, 5.0, 1.0);	
	return 0;
}
