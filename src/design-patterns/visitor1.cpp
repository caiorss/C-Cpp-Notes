// Brief:  Simple visitor design pattern for processing heterogeneous derived classes.
// Author: Caio Rodrigues
//------------------------------------------------------------------

#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <functional>

// Forward reference
//class IVisitor;
class Circle;
class Square;
class Blob;

// File: IVisitor.h 
// Visitor: The visitor interface encapsulates the operation performed
// on the class hierarchy. It should define a specific version operation
// (method visit) for each derived class. 
class IVisitor{
public:
	virtual ~IVisitor() = default;
	virtual void visit(Circle& sh) = 0;
	virtual void visit(Square& sh) = 0;
	virtual void visit(Blob& sh) = 0;
};

// Shape interface - The base class must define the method accept
class IShape{
public:
	virtual ~IShape() = default;	
	virtual void accept(IVisitor& v) = 0;
};

// ============ Concrete shapes ======= //

class Circle: public IShape{
public:
	double radius;
	Circle(double radius): radius(radius) { }
	void accept(IVisitor& v) override {
		v.visit(*this);
	}
};

class Square: public IShape{
public:
	double side;
	Square(double side): side(side) { }
	void accept(IVisitor& v) override {
		v.visit(*this);
	}
};

class Blob: public IShape{
public:
	Blob(){}
	void accept(IVisitor& v) override {
		v.visit(*this);
	}
};

// ======== Visitor Interface Implementation =============//
// Every visitor is a new operation to be performed on the
// class hierarchy

// Operation which prints name of the class hierarchy 
class PrintNameVisitor: public IVisitor{
public:
	void visit(Circle& sh) override
	{
		std::cout << " => Shape is a circle of radius = "
				  << sh.radius
				  << "\n";
	}
	void visit(Square& sh) override
	{
		std::cout << " => Shape is a square of side = "
				  << sh.side
				  << "\n";		
	}
	void visit(Blob& ) override
	{
		std::cout << " => Shape is a blob with an undefined shape"
				  << "\n";
	}
};

class ComputeAreaVisitor: public IVisitor{
private:
	double _area = 0.0;
public:
	ComputeAreaVisitor() = default;
	double getArea(){
		return _area;
	}
	void visit(Circle& sh) override
	{
		// CircleArea =  PI * radius^2
		_area = 3.1415 * sh.radius * sh.radius;
	}
	void visit(Square& sh) override
	{
		_area = sh.side * sh.side;
	}
	void visit(Blob& ) override
	{
		_area = 100.0;
	}
};

// Lambda function are used for adding new behavior to the object.
template<class Result>
class FunctionAdapter: public IVisitor{
	template<typename T> using Func = std::function<Result (T& sh)>;
	Result       _res;
	Func<Circle> _fn_circle;
	Func<Square> _fn_square;
	Func<Blob>   _fn_blob;
public:
	Result get(){
		return _res;
	}
	FunctionAdapter(Func<Circle> fnCircle, Func<Square> fnSquare, Func<Blob> fnBlob)
		: _res{}
		, _fn_circle{fnCircle}
		, _fn_square{fnSquare}
		, _fn_blob{fnBlob}
	{		
	}		
	void visit(Circle& sh) override { _res = _fn_circle(sh); }
	void visit(Square& sh) override { _res = _fn_square(sh);}
	void visit(Blob& sh)   override { _res = _fn_blob(sh); }
};


int main()
{
	// Sample shapes 
	auto s1 = Circle(3.0);
	auto s2 = Square(4.0);
	auto s3 = Blob();

	std::cout << "===> Experiment 1: PrintNameVisitor " << "\n";
	auto visitor1 = PrintNameVisitor();
	s1.accept(visitor1);
	s2.accept(visitor1);
	s3.accept(visitor1);	

	std::cout << "===> Experiment 2: ComputeAreaVisitor " << "\n";
	auto visitor2 = ComputeAreaVisitor();
	s1.accept(visitor2);
	std::cout << "Area of shape 1 = " << visitor2.getArea() << "\n";
	s2.accept(visitor2);
	std::cout << "Area of shape 2 = " << visitor2.getArea() << "\n";
	s3.accept(visitor2);
	std::cout << "Area of shape 3 = " << visitor2.getArea() << "\n";

	std::cout << "===> Experiment 3: FunctionAdapter " << "\n";
	// Creates operation to get shape name as string 
	auto visitor3 = FunctionAdapter<std::string>{
		[](Circle& ){ return "circle"; },
		[](Square& ){ return "square"; },
		[](Blob&   ){ return "blob";   },
	 };
	s1.accept(visitor3);
	std::cout << "Type of shape 1 = " << visitor3.get() << "\n";
	s2.accept(visitor3);
	std::cout << "Type of shape 2 = " << visitor3.get() << "\n";
	s3.accept(visitor3);
	std::cout << "Type of shape 3 = " << visitor3.get() << "\n";

	std::cout << "===> Experiment 4: FunctionAdapter " << "\n";
	// Creates operation to compute shape perimeter
	auto visitor4 = FunctionAdapter<double>{
		[](Circle& s){ return 2 * 3.1415 * s.radius * s.radius ; },
		[](Square& s){ return 4.0 * s.side; },
		[](Blob&    ){ return -100.0;   },
	 };	
	s1.accept(visitor4);
	std::cout << "Perimeter of shape 1 = " << visitor4.get() << "\n";
	s2.accept(visitor4);
	std::cout << "Perimeter of shape 2 = " << visitor4.get() << "\n";
	s3.accept(visitor4);
	std::cout << "Perimeter of shape 3 = " << visitor4.get() << "\n";
	
	return 0;
}
