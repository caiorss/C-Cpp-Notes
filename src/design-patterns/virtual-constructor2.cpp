// File:   virtual-consructor2.cpp
// Brief:  Enhanced virtual constructor idiom with CRTP design pattern. 
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
	virtual auto clone() const -> std::unique_ptr<IBase> = 0;
	// "virtual default constructor"
	virtual auto create() const        -> std::unique_ptr<IBase> = 0;	
	virtual auto getID() const         -> std::string  = 0;
	virtual auto setID(std::string id) -> void		   = 0;
	virtual auto show() const          -> void		   = 0;
};

/** Remember: Template always in header files. */
template<typename Base, typename Derived>
class Copyable: public Base{
private:
	inline auto self() const -> const Derived& {
		return *static_cast<const Derived* const>(this);
	}
public:
	virtual ~Copyable() = default;
	auto clone() const -> std::unique_ptr<Base> {
		// Invoke copy constructor 
		return std::make_unique<Derived>(this->self());
	}
	auto create() const -> std::unique_ptr<Base> {
		// Invoke default constructor 
		return std::make_unique<Derived>();
	}
};

class DerivedA: public Copyable<IBase, DerivedA>{
private: 
	std::string _id;
public:
	DerivedA()
		: _id("unnamed-A"){ }
  
	DerivedA(std::string id)
		: _id{std::move(id)}{  }
  
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

class DerivedB: public Copyable<IBase, DerivedB>{
private:
	std::string _id;
public:
	DerivedB()
		: _id("unnamed-B"){ }
  
	DerivedB(std::string id)
		: _id{std::move(id)}{  }
  
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
