// Brief:  Generic visitor pattern implemented with C++17 variants 
// Author: Caio Rodrigues
//------------------------------------------------------------------

#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <functional>

#include<type_traits>

// C++17 Variant std::variant and std::visit 
#include <variant> 

class Circle{
public:
	double radius;
	Circle(double radius): radius(radius) { }
};

class Square{
public:
	double side;
	Square(double side): side(side) { }
};

class Blob{
public:
	Blob(){}
};

// ======== Generic Visitor =============//

/** Basic visitor */
struct PrintNameVisitor{
	void operator()(const Circle& sh){
		std::cout << " SHAPE = Circle of radius " << sh.radius << std::endl;
	}
	void operator()(const Square& sh){
		std::cout << " SHAPE = Square of radius " << sh.side << std::endl;
	}
	void operator()(const Blob&){
		std::cout << " SHAPE = Blob - no one knows how it looks like. " << std::endl;
	}	
};

/** Visitor which adpts a function */
template<typename Result>
class FunctionVisitor{
private:	
	template<typename Input> using FN = std::function<Result (const Input&)>;
	FN<Circle> fn_circle;
	FN<Square> fn_square;
	FN<Blob>   fn_blob;
public:
	//Result     result;
	FunctionVisitor(FN<Circle> fnCircle, FN<Square> fnSquare, FN<Blob> fnBlob)
		: fn_circle(fnCircle)
		 ,fn_square(fnSquare)
		 ,fn_blob(fnBlob){ }
	//Result get() const { return result; }
	Result operator()(const Circle& sh){ return fn_circle(sh); }
	Result operator()(const Square& sh){ return fn_square(sh); }
	Result operator()(const Blob& sh){  return fn_blob(sh);   }
};


template<typename Input, typename Result>
using FnVisit = std::function<Result (const Input&)>;

/** Make a visitor using a functional-programming way 
 * Visitor with lambdas look like "pattern matching"  
 * from functional languages.
 */
template<typename Result = void>
auto makeVisitor(FnVisit<Circle, Result> fnCircle,
				 FnVisit<Square, Result> fnSquare,
				 FnVisit<Blob, Result>   fnBlob ){
	return [=](auto&& a){
			   using C = std::decay_t<decltype(a)>;
			   if constexpr(std::is_same_v<C, Circle>){               
                    return fnCircle(a);
               }
			   if constexpr(std::is_same_v<C, Square>){
                    return fnSquare(a);
               }
               if constexpr(std::is_same_v<C, Blob>){               
                   return fnBlob(a);
               }
			   std::cerr << "WARNING - Not returned valid result";
               return Result{};
		   };
} //-- EoF makeVisitor ----// 


int main()
{
	using Shape = std::variant<Circle, Square, Blob>;	
	
	// Sample shapes
	Shape s1 = Circle(3.0);
	Shape s2 = Square(4.0);
	Shape s3 = Blob();

	auto printVisitor = PrintNameVisitor();

	std::puts("\n === EXPERIMENT 1 =================");
	std::visit(printVisitor, s1);
	std::visit(printVisitor, s2);
	std::visit(printVisitor, s3);

	std::puts("\n === EXPERIMENT 2 =================");
	auto visitorGetName = FunctionVisitor<std::string>{
		[](const Circle& ) { return "circle"; },
		[](const Square& ) { return "square"; },
		[](const  Blob&  ) { return "blob";   }
	};

	std::cout << "Type of shape 1 = " << std::visit(visitorGetName, s1) << "\n";
	std::cout << "Type of shape 2 = " << std::visit(visitorGetName, s2) << "\n";
	std::cout << "Type of shape 3 = " << std::visit(visitorGetName, s3) << "\n";

	std::puts("\n === EXPERIMENT 3 =================");
	
	// Creates operation to compute shape perimeter
	auto visitorGetArea = FunctionVisitor<double>{
		[](const Circle& s){ return 3.1415 * s.radius * s.radius ; },
		[](const Square& s){ return s.side * s.side; },
		[](const Blob&    ){ return 0.0 / 0.0; /* NAN Not a number */   },
	};

	std::cout << "Area of shape 1 = " << std::visit(visitorGetArea, s1) << "\n";
	std::cout << "Area of shape 2 = " << std::visit(visitorGetArea, s2) << "\n";
	std::cout << "Area of shape 3 = " << std::visit(visitorGetArea, s3) << "\n";

	std::puts("\n === EXPERIMENT 4 =================");

	auto fnVisitorArea = makeVisitor<double>(
	  	 [](const Circle& s){ return 3.1415 * s.radius * s.radius ; }
		,[](const Square& s){ return s.side * s.side; }
		,[](const Blob&    ){ return 0.0 / 0.0; /* NAN Not a number */   }
		);

	std::cout << "Area of shape 1 = " << std::visit(fnVisitorArea, s1) << "\n";
	std::cout << "Area of shape 2 = " << std::visit(fnVisitorArea, s2) << "\n";
	std::cout << "Area of shape 3 = " << std::visit(fnVisitorArea, s3) << "\n";
	
	return 0;
}
