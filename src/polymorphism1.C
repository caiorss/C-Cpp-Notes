// CLING/ROOT Script for testing OOP polymorphism in C++
// Author: Caio Rodrigues
//-------------------------------------------------------
#include <iostream>
#include <string> 
#include <memory>

class Base{
public:
	Base(){}
	// Copy constructor
	Base(const Base& rhs){
		std::cerr << " [TRACE] " << __FILE__ << ":" << __LINE__ << " "
				  << " Base copy constructor invoked." << "\n";
	}
	// Copy assignment operator
	Base& operator= (const Base& rhs){
		std::cerr << " [TRACE] " << __FILE__ << ":" << __LINE__ << " " 
				  << " Base copy assignment operator invoked." << "\n";
		return *this;
	}
	// The base class always need a virtual
	// destructor 
	virtual ~Base() = default;	
	
	virtual std::string getType() const {
		return "Base";
	}	
	std::string getType2() const {
		return "Base";
	}
	void showType(){
		//OR: std::cout << "Class type = " << getType() << "\n";
		std::cout << "Class type = " << this->getType() << "\n";
	}
};

class DerivedA: public Base{
public:
	DerivedA(){}
	std::string getType() const {
		return "DerivedA";
	}
	std::string getType2() const {
		return "DerivedA";
	}		
};

class DerivedB: public Base{
public:
	DerivedB(){}
	std::string getType() const {
		return "DerivedB";
	}
	std::string getType2() const {
		return "DerivedB";
	}		
};

// Pass by value 
void printClassNameValue(Base obj){
	std::cout << "Object type is: " << obj.getType() << "\n";
}
// Pass by pointer 
void printClassNamePtr(const Base* obj){
	std::cout << "Object type is: " << obj->getType() << "\n";
}
// Pass by reference 
void printClassNameRef(const Base& obj){
	std::cout << "Object type is: " << obj.getType() << "\n";
}

 // This factory functon will fail due to object slicing 
Base factoryFunctionValue(const std::string& type){
	if(type == "base")
		return Base();
	if(type == "da")
		return DerivedA();
	if(type == "db")
		return DerivedB();
	throw std::runtime_error("Error: class type not found.");
}

// Fails because the object p is destroyed when
// it goes out of scope. Any attempt to use the
// returned reference results in segmentation fault.
Base& factoryFunctionRef(const std::string& type){
	Base p;
	if(type == "base")
		p = Base();
	else if(type == "da")
		p = DerivedA();
	else if(type == "db")
		p = DerivedB();
	else throw std::runtime_error("Error: class type not found.");
	return p;
}

// Works, however this version returning a raw pointer
// is prone to memory leaks.
Base* factoryFunctionRawPointer(const std::string& type){
	if(type == "base")
		return new Base();
	else if(type == "da")
		return new DerivedA();
	if(type == "db")
		return new DerivedB();
	return nullptr;
}

// Requires header: <memory>
std::unique_ptr<Base> factoryFunctionSafe(const std::string& type){
	if(type == "base")
		return std::make_unique<Base>();
	else if(type == "da")
		return std::make_unique<DerivedA>();
	if(type == "db")
		return std::make_unique<DerivedB>();
	return nullptr;
}
