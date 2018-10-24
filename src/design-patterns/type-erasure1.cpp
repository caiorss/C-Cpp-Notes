// File:      type-erasure.cpp 
// Brief:     Basic type erasure implementation example.
// Objective: Demonstrate how type erasure works.
// Author:    Caio Rodrigues
//-------------------------------------------------------
#include <iostream>
#include <vector>
#include <deque>
#include <memory> // Smart pointers 

class TypeErasure{
private:
	//  --- Forward declarations ----
	class Concept;
	template<class T> class Model;

	// --- Member Variables ----- // 
	std::shared_ptr<Concept> _concept_ptr;
	// Optional: 
	// RTTI (runtime type information) for recovering wrapped type
	// by downcasting 
	const std::type_info& _tinfo;
public:
	template<typename T> 
	TypeErasure(const T& obj)
        : _concept_ptr(std::make_shared<Model<T>>(obj))
        ,_tinfo(typeid(T)) 
	{		
	}

	auto getName() const -> std::string {
		return _concept_ptr->getName();
	}

	// Recover reference to wrapped type 
	template<typename T>
	auto recover() -> T {		
		if(typeid(T) != _tinfo)
			throw std::runtime_error("Error: cannot cast to this type");
		// Note: static_cast downcasting to wrong type has undefined behavior,
		// use with care!
		return static_cast<Model<T>*>(_concept_ptr.get())->_obj;
	}

	template<typename T>
	auto hasType() -> bool {
		return _tinfo == typeid(T);
	}
private:
	// Concept class defines the interface to be enforced
	// In general, it is an interface class, a class with only pure virtual
	// methods (abstract methods), in other words methods without implementation.
	class Concept{
	public:
		virtual auto getName() const -> std::string = 0;
		virtual ~Concept() = default;
	};

	// Adapt the wrapped type (T) to the concept
	template<typename T>
	class Model: public Concept {
	public:
		// Instance of the wrapped type 
		T _obj;
		// Initialize _opj by copying the parameter 
		Model(const T& obj): _obj(obj){}
		auto getName() const -> std::string {
			return _obj.getName();
		}
	};
};


class A{
public:
	std::string getName() const {
		return "class A";
	}
	void sayA(){
		std::cout << "I am the class A" << "\n";
	}
};

class B{
public:
	std::string getName() const  {
		return "class B";
	}
	void sayB(){
		std::cout << "I am the class B" << "\n";
	}	
};

class C{
public:
	std::string getName() const  {
		return "class C";
	}
	void sayC() const {
		std::cout << "I am the class C" << "\n";
	}
};

int main(){
	auto tlist = std::deque<TypeErasure>();
	tlist.emplace_back(A());
	tlist.emplace_back(B());
	tlist.emplace_back(C());

	std::cout << "\n" << "EXPERIMENT 1 ============" << "\n";	
	for(const auto& t: tlist) {
		std::cout << "Class type = " << t.getName() << "\n";
	}

	 // Note: It is a copy!
	 A objA = tlist.at(0).recover<A>();
	 objA.sayA();

	 std::cout << "\n" << "EXPERIMENT 2 ============" << "\n";
	 
	 // Simulate downcasting failure 
	 try {
		 B objB = tlist.at(0).recover<B>();
		 objB.sayB();
	 } catch(const std::runtime_error& ex){
		 std::cout << " [FAILURE]" << ex.what() << "\n";
	 }

	 B objB = tlist.at(1).recover<B>();
	 objB.sayB();

	 auto objC = tlist.at(2).recover<C>();
	 objC.sayC();
	 
	 return EXIT_SUCCESS;
}
