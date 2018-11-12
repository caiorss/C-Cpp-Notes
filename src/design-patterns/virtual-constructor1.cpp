// File:   virtual-consructor1.cpp
// Brief:  Demonstration of a simple implementation of virtual copy constructor.
// Author: Caio Rodrigues 
// ------------------------------------------------------------------

#include <iostream>
#include <string>
#include <memory>

class IBase{
public:
	// Destructor always virtual to avoid memory leak,
	virtual ~IBase() = default;	
	// "virtual copy constructor"
	virtual auto clone() const -> IBase* = 0;
	// "virtual default constructor"
	virtual auto create() const        -> IBase*	   = 0;	
	virtual auto getID() const         -> std::string  = 0;
	virtual auto setID(std::string id) -> void		   = 0;
	virtual auto show() const          -> void		   = 0;
};

class DerivedA: public IBase{
private: 
	std::string _id;
public:
	DerivedA()
		: _id("unnamed-A"){ }
  
	DerivedA(std::string id)
		: _id{std::move(id)}{  }
  
	auto clone() const -> IBase* {
		// Invoke copy constructor 
		return new DerivedA(*this);
	}
	auto create() const -> IBase* {
		// Invoke default constructor 
		return new DerivedA();
	}
	auto getID() const -> std::string {
		return _id;
	}
	auto setID(std::string id) -> void {
		_id = id;
	}   
	auto show() const -> void  {
		std::cout << " => Class DerivedA - id = " << _id << "\n";
	}  
};


class DerivedB: public IBase{
private:
	std::string _id;
public:
	DerivedB()
		: _id("unnamed-B"){ }
  
	DerivedB(std::string id)
		: _id{std::move(id)}{  }
  
	auto clone() const -> IBase* {
		// Invoke copy constructor 
		return new DerivedB(*this);
	}
	auto create() const -> IBase* {
		// Invoke default constructor 
		return new DerivedB();
	}  
	auto getID() const -> std::string {
		return _id;
	}
	auto setID(std::string id) -> void {
		_id = std::move(id);
	}     
	auto show() const -> void  {
		std::cout << " => Class DerivedB - id = " << _id << "\n";
	}  
};

int main(){
  
	return 0;
}
