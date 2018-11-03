// File:   factory-universal1.cpp1
// Author: Caio Rodrigues  
// Brief:  Universal Object Factory which can instantiate objects of any type.
// Note:   The benefit of this implementation is that object doesn't need to have
//         the same base class and the factory has no knowledge about any base class.
//----------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <memory>

#define RUNTIME_ERROR_LOCATION(message)  \
	runtime_error_location(__LINE__, __FILE__, message)

// Register a non-polymorphic class -> It means a class without base class 
#define REGISTER_CLASS_TO_FACTORY1(aclass)								\
	namespace {  auto register_ ## derived = FactoryImpl<aclass, aclass>( #aclass ); }

// Register a polymorphic class to the factory.
#define REGISTER_CLASS_TO_FACTORY2(derivedClass, baseClass) \
	namespace {  auto register_ ## derivedClass = FactoryImpl<derivedClass, baseClass>( #derivedClass ); }

// Custom exception with better error message and context information
// such as file and line number where the exception was thrown.
struct runtime_error_location: public std::exception {
	std::string  message;
    runtime_error_location(const std::string& text)
		: message{text}
	{
	}
    runtime_error_location(
		unsigned int line,
		const std::string& file,
		const std::string& text
		)
	{
		message = file + ":" + std::to_string(line) + ": " + "Error: " + text;
	}
    auto what() const noexcept -> const char* {
		return message.c_str();
    }
};

class UniversalFactory{
private:
	using FactoryMap = std::map<std::string, UniversalFactory*>;
	// Force global variable to be initialized, thus it avoid
	// the inialization order fisaco.
	static auto getRegister() -> FactoryMap& {
		static FactoryMap classRegister{};
		return classRegister;
	}
public:
      	// ========== Instance Methods ========//

	// Destructor
	virtual
	~UniversalFactory() = default;

	// Encapsulates class constructor.
	// The type void* performs pointer type erasure
	virtual
	auto create() const -> void* = 0;

	// Encapsulates destructor 
	virtual
	auto destroy(void* object) const -> void = 0;

	virtual
	auto size() const -> size_t = 0;
	
	virtual
	auto typeinfo() const -> const std::type_info& = 0;

      	// ========== Static Methods ========//

	/** Register factory object of derived class */
	static
	auto registerFactory(const std::string& name, UniversalFactory* factory) -> void {
		auto& reg = UniversalFactory::getRegister();
		reg[name] = factory;
	}
	/** Show all registered classes printing their name to stdout. */
	static
	auto showClasses() -> void {
		std::cout << " Registered classes. " << "\n";
		std::cout << " =================== " << "\n";
		for(const auto& pair: UniversalFactory::getRegister())
			std::cout << " + " << pair.first
					  << " ; RTTI name = "
					  << pair.second->typeinfo().name()
					  << " ; size (bytes) = "
					  << pair.second->size()
					  << "\n";
	}

	/** Attemp to instantiate a class, if it is not possible, returns nullptr */
	template<class BaseClass>
	static
	auto make(const std::string& name) -> std::unique_ptr<BaseClass> {
		FactoryMap& reg = UniversalFactory::getRegister();
		auto it = reg.find(name);
		if(it == reg.end())
			return nullptr;
		// Avoid core dump if the conversion is not possible.
		if(it->second->typeinfo() != typeid(BaseClass))
			return nullptr;
		void* ptr = it->second->create();
		return std::unique_ptr<BaseClass>(reinterpret_cast<BaseClass*>(ptr));
	}

    /** Attempt to instantiate class, if it is not possible throws an exception. */
	template<class BaseClass>
	static
	auto makeSafe(const std::string& name) -> std::unique_ptr<BaseClass> {
		// Throw exception for providing better context information and avoid
		// Core dump due to dangerous reinterpret_cast<>.
		auto object = UniversalFactory::make<BaseClass>(name);
		if(object == nullptr)
			throw RUNTIME_ERROR_LOCATION(
				std::string("Cannot create type. Failed to cast void* to: ") + name	);
		return object;
	}
};  // -------- End Of class UniversalFactory() ------//

/** Register derived class from a given class hierarchy. 
 *  It also captures type metadata. 
 */
template<typename DerivedClass, typename BaseClass = DerivedClass>
class FactoryImpl: UniversalFactory{
private:
	const std::type_info& _tinfo;
public:
	// Register this global object on the Factory register
	FactoryImpl(const std::string& name)
		: _tinfo(typeid(BaseClass))
	{
		std::cerr << " [TRACE] " << " Registered Class = " << name << "\n";
		UniversalFactory::registerFactory(name, this);
	}
	// Capture class default constructor => constructor type erasure 
	auto create() const -> void* override {
		return new DerivedClass;
	}
	// Capture destructor => Destructor type erasure 
	auto destroy(void* object) const -> void override {
		delete reinterpret_cast<BaseClass*>(object);
	}	
	auto typeinfo() const -> const std::type_info& override {
		return _tinfo;
	}
	auto size() const -> size_t override {
		return sizeof(DerivedClass);
	}
};

// ================ Factory Products =================== //

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
REGISTER_CLASS_TO_FACTORY2(Base, Base);

class DerivedA: public Base{
public:
    DerivedA(){}
    auto getType() const  -> std::string {
       return "DerivedA";
    }
};
REGISTER_CLASS_TO_FACTORY2(DerivedA, Base);

class DerivedB: public Base{
public:
	DerivedB(){}
	auto getType() const -> std::string {
      return "DerivedB";
   }
};
REGISTER_CLASS_TO_FACTORY2(DerivedB, Base);

class NonDerived{
public:
	NonDerived(){}
	void printMessage(){
		std::puts(" ==> I am a non derived class");
	}
};
REGISTER_CLASS_TO_FACTORY1(NonDerived);

// Register class to factory before initizliation
// Note: It can be better automated with a macro.
#if 0
namespace {
	auto register_Base       = FactoryImpl<Base, Base>("Base");
	auto register_DerivedA   = FactoryImpl<DerivedA, Base>("DerivedA");
	auto register_DerivedB   = FactoryImpl<DerivedB, Base>("DerivedB");
	auto register_NonDerived = FactoryImpl<NonDerived>("NonDerived");
}
#endif 

int main(){
	UniversalFactory::showClasses();	
	std::cout << "------- Test 1 ------------" << "\n";
	auto base = UniversalFactory::make<Base>("Base");
	base->showType();
	auto da = UniversalFactory::make<Base>("DerivedA");
	da->showType();
	auto db = UniversalFactory::make<Base>("DerivedB");
	db->showType();
	
	std::cout << "------- Test 2 ------------" << "\n";
	
	for(const auto& cls : {"Base", "DerivedA", "DerivedB"} ){
		auto obj = UniversalFactory::makeSafe<Base>(cls);
		std::cout << "Class [" << cls << " ] = " << obj->getType() << " \n";
	}
	
	std::cout << "------- Test 3 ------------" << "\n";
	auto ndc = UniversalFactory::make<NonDerived>("NonDerived");
	ndc->printMessage();

	std::cout << "------- Test 4 ------------" << "\n";
	// Simulate exception
	try {
		auto dummy = UniversalFactory::makeSafe<Base>("NonDerived");
		dummy->showType();
	} catch(const runtime_error_location& ex){
		std::cerr << " [ERROR] " << ex.what() << "\n";
	}

	return 0;
}
