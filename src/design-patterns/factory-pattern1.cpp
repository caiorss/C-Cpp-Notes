// File:   factory-pattern1.cpp 
// Brief:  Factory design pattern with self registering of derived classes.
// Author: Caio Rodrigues
//=======================================================================

#include <iostream>
#include <string>
#include <map>
#include <memory>

// Macro for class registration 
#define REGISER_FACTORY(derivedClass) \
	namespace { auto registry_ ## derivedClass = ConcreteFactory<derivedClass>(#derivedClass);  }

          // ========>>> Forward declarations =====//
class Base;
class DerivedA;
class DerivedB;

          // ============  Factory Infrastructure =========== //

class Factory{
private:
	using FactoryMap = std::map<std::string, Factory*>;
	// Force global variable to be initialized, thus it avoid
	// the inialization order fisaco. 
	static auto getRegister() -> FactoryMap& {
		static FactoryMap classRegister{};
		return classRegister;
	}
public:	
	/** Register factory object of derived class */
	static
	auto registerFactory(const std::string& name, Factory* factory) -> void {
		auto& reg = Factory::getRegister();
		reg[name] = factory;
	}
	/** Show all registered classes */
	static
	auto showClasses() -> void {
		std::cout << " Registered classes. " << "\n";
		std::cout << " =================== " << "\n";
		for(const auto& pair: Factory::getRegister())
			std::cout << " + " << pair.first << "\n";
	}		
	/**  Construct derived class returning a raw pointer */
	static
	auto makeRaw(const std::string& name) -> Base* {
		auto it = Factory::getRegister().find(name);
		if(it != Factory::getRegister().end())
			return it->second->construct();
		return nullptr;
	}
	
    /** Construct derived class returning an unique ptr  */
	static
	auto makeUnique(const std::string& name) -> std::unique_ptr<Base>{
		return std::unique_ptr<Base>(Factory::makeRaw(name));
	}

	// Destructor 
	virtual
	~Factory() = default;
	
	virtual
	auto construct() const -> Base* = 0;
};

template<typename DerivedClass>
class ConcreteFactory: Factory{
public:
	// Register this global object on the Factory register 
	ConcreteFactory(const std::string& name){
		std::cerr << " [TRACE] " << " Registered Class = " << name << "\n";
		Factory::registerFactory(name, this);
	}
	auto construct() const -> Base* {
		return new DerivedClass;
	}	
};


// ========>>> Factory Products - or concrete classes created by factory <=======//

class Base{
public:
	Base() = default;
	// Destructor of base class must always be virtual 
	virtual ~Base() = default;   
	virtual auto getType() const -> std::string {
      return "Base";
   }	
   void showType(){
      std::cout << "Class type = " << this->getType() << "\n";
   }
};

// Register Base class 
namespace {
	// Anonymous namespace is used to make the definitions here private to the current
	// compilation unit (current file). It is equivalent to the old C static keyword.
	// It could be placed at Base.cpp 
	ConcreteFactory<Base> factoryBase("Base");
}

class DerivedA: public Base{
public:
    DerivedA(){}
    auto getType() const  -> std::string {
       return "DerivedA";
    }
};

//  Registration with macro. 
REGISER_FACTORY(DerivedA);

class DerivedB: public Base{
public:
	DerivedB(){}
	auto getType() const -> std::string {
      return "DerivedB";
   }
};

REGISER_FACTORY(DerivedB);

// Disadvantage: Adding new derived classes to this function requires code modification
auto simpleFactory(const std::string& name) -> std::unique_ptr<Base> {
	if(name == "Base")
		return std::make_unique<Base>();
	if(name == "DerivedA")
		return std::make_unique<DerivedA>();
	if(name == "DerivedB")
		return std::make_unique<DerivedB>();
	return nullptr;
}

int main(){

	Factory::showClasses();
	std::cout << "\n";
	std::unique_ptr<Base> objBase = Factory::makeUnique("Base");
	std::cout << " type of objBase = " << objBase->getType() << "\n";

	std::unique_ptr<Base> objDA = Factory::makeUnique("DerivedA");
	std::cout << " type of derivedA = " << objDA->getType() << "\n";

	std::unique_ptr<Base> objDB = Factory::makeUnique("DerivedB");
	std::cout << " type of derivedA = " << objDB->getType() << "\n";

	std::unique_ptr<Base> objDC = Factory::makeUnique("Derived-error");
	if(!objDC)
		std::cout << " ==> Error: object not found in factory" << '\n';
	
	return 0;
}
