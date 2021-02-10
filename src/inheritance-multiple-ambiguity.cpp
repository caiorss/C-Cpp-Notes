// File:   inheritance-multiple-ambiguity.cpp
// Brief:  General code to test how to solve C++ multiple inheritance ambiguity.
// Author: Caio Rodrigues 
#include <iostream>
#include <string> 

class BaseA{
protected:
	std::string key = "KEY-BASEA";
public:	
	
	std::string _name;

	BaseA(std::string name): _name(name) {
		std::cerr << " [LOG] BaseA instantiated. OK." << "\n";
	}
	// A base class always needs virtual constructor in order to avoid memory leaks.
	virtual ~BaseA() = default;
	
	void printName(){
		std::cout << " A - [printName] => Name = " << _name << "\n";
	}	
	void printNameA(){
		std::cout << " [printNameA] => NameA = " << _name << "\n";
	}
	void callOverridenVFun(){
		std::cout << " [BaseB::callOverridenVFun] BaseB::overridenVfun() returned"
				  << '\n' << " +=> RETURN = " << this->overridenVfun() << "\n";
	}
	virtual std::string overridenVfun() = 0;	

	virtual std::string nonOverridenVfun(){
		return "BaseA::nonOverridenVfun";
	}

	virtual std::string specificAVFun(){
		return "specificAVFun not overriden.";
	}

	static void staticMethod(){
		std::cout << " [BaseA] I am a static method from BaseA" << "\n";
	}
	
};

class BaseB{
	// Only derived classes can access 
protected: 
	std::string key = "KEY-BASEB";
	
   // Derived classes and any external code can access	
public:    
	std::string _name;

	BaseB(std::string name): _name(name) {
		std::cerr << " [LOG] BaseB instantiated. OK." << "\n";
	}

	virtual ~BaseB() = default;
	
	void printName(){
		std::cout << " B - [printName] => Name = " << _name << "\n";
	}
	void printNameB(){
		std::cout << " [PrintName] => NameB = " << _name << "\n";
	}

	void callOverridenVFun(){
		std::cout << " [BaseB::callOverridenVFun] BaseB::overridenVfun() returned"
				  << '\n' << " +=> RETURN = " << this->overridenVfun() << "\n";
	}
	virtual std::string overridenVfun(){
		return " BaseB::overridenVfun not overriden";
	}	
	virtual std::string nonOverridenVfun(){
		return "BaseB::nonOverridenVfun";
	}
	virtual std::string specificBVFun(){
		return "specificBVFun not overriden.";
	}

	static void staticMethod(){
		std::cout << " [BaseB] I am a static method from BaseB" << "\n";
	}
};

class Derived: public BaseA, public BaseB{	
public:
	// Base class constructos must always be initialized through
	// initialization list like this.
	Derived(): BaseA("nameA"), BaseB("nameB")
	{		
	}
	Derived(std::string nameA, std::string nameB)
		: BaseA(nameA), BaseB(nameB)
	{		
	}
	~Derived(){
		std::cout << " [Derived] Destructor called. " << "\n";
	}
	// Access ambiguous protected field from base class 
	void showKeyBaseA(){
		/** error: member 'key' found in multiple base classes of different types
         *  std::cout << " Key-BaseA = " << this->key << "\n";
		 */
		std::cout << " [showKeyBaseA] Key-BaseA = " << BaseA::key << "\n";
		std::cout << " [showKeyBaseA] Key-BaseA = " << this->BaseA::key << "\n";
	}
	void showKeyBaseB(){
		std::cout << " [showKeyBaseB] Key-BaseB = " << BaseB::key << "\n";
		std::cout << " [showKeyBaseB] Key-BaseB = " << this->BaseB::key << "\n";
	}

	void printNameX(){
		BaseA::printName();
		// Or 'this' pointer can be used too.
		this->BaseB::printName();
	}
	
	/* The derived class overrides both virtual member functions, BaseA::overridenVfun()
     * and BaseB::overridenVfun(). As a result, both base classes will share the derived class 
     * implementation.
     */
    #if true
	std::string overridenVfun() override {
		return "Both functions overriden, BaseA::overridenVfun() and BaseB::overridenVfun(). OK.";
	}
	#endif 

	// std::string specificAVFun() override {
	std::string specificAVFun() override{
		return "Overriden: specificAVFun";
	}
	std::string specificBVFun() override {
		return "Overriden: specificBVFun";
	}

	static void staticMethod(){
		std::cout << " [Derived] I am a static method from Derived" << "\n";
	}
};

int main(){
	const char nl = '\n';
	std::cout << " [LOG] Start program OK." << nl;
	Derived d;

	std::puts("\nEXPERIMENT 1 - Access ambiguous base class public field");
	
	/* multiple-inheritance.cpp:43:37: error: member '_name' found in 
     * multiple base classes of different types 
     * CODE: std::cout << "d.name (BaseA)" << d._name << nl;
     */
	// Access public field, _name inherited from BaseA 
	std::cout << " => d.name (BaseA) = " << d.BaseA::_name << nl;

    // Access public field, _name inherited from BaseB 
	std::cout << " => d.name (BaseB) = " << d.BaseB::_name << nl;	

	std::puts("\nEXPERIMENT 2 - Call non ambiguous member function from base classes.");
	// The class Derived inherits (public inheritance) everything 
	// from BaseA and BaseB. 
	d.printNameA();
	d.printNameB();
	
	std::puts("\n EXPERIMENT 3 - Call ambiguous member functions from base classes.");
	// Ambiguous method-call resolution
	// error: member 'printName' found in multiple base classes of different types
	// d.printName();
	//-----------------------------------------------------
	d.BaseA::printName();
	d.BaseB::printName();
	
	std::puts("\n EXPERIMENT 4 - Call member functions which access ambiguous fields.");
	d.showKeyBaseA();
	d.showKeyBaseB();

	std::puts("\n EXPERIMENT 5 - Call member functions which access ambiguous fields.");
	Derived* ptr = &d;
	ptr->BaseA::callOverridenVFun();

	ptr->BaseB::callOverridenVFun();

	
	return 0;
}
