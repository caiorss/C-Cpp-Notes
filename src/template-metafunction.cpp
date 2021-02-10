// File:   template-metafunction.cpp
// Brief:  Template metaprogramming with metafunctions for type introspection.
// Author: Caio Rodrigues
//====================================================================================
#include <iostream>
#include <iomanip>    
#include <vector>
#include <cmath>      
#include <functional> 
#include <string>

#define disp(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  "\n"

// ====== Meta function to determine whether type is pointer ==============//
/* Metafunction which checks whether type is pointer  */
template<class T>
struct isPointer{
	/* => Without constexpr, we got this error message with 
     *       static bool value = false;
     *  "error: non-const static data member must be initialized out of line"
     */
	static constexpr bool value = false;
	constexpr bool operator()() const { return false; }
};

template<class T>
struct isPointer<T*>{
	static constexpr bool value = true;
	constexpr bool operator()() const { return true; }
};

//===== Old C++ metafunctions without constexpr =============

template<class T>
struct isPointerOLD{
	enum { value = 0 };
	// Note: non const static member functions must be
	// initilialized out of the class declaration.
	static const bool result = false;
};

template<class T>
struct isPointerOLD<T*>{
	enum { value = 1 };
	static const bool result = true;	
};


// ====== Meta function to determine whether type is const ==============//

template<class T> struct isConst{
	static constexpr bool value = false;
};
template<class T> struct isConst<const T>{
	static constexpr bool value = true;
}; 
template<class T> struct isConst<const T*>{
	static constexpr bool value = true;
};
template<class T> struct isConst<const T&>{
	static constexpr bool value = true;
};

// ====== Meta function for type transformation  ==============//
// Technique: Metafunction idiom + Type generation + Template partial specilization
template<class T> struct removePointer{
	typedef T type;
};

template<class T> struct removePointer<T*>{
	typedef T type;
};


// ====== Meta function to determine whether type is pointer ==============//
//
/** Meta function for computing type information at compile-time 
 * Note: It is possible also to use: template<class T>
 *------------------------------------------------------- */
template<typename T>
struct Typeinfo{
	// The advantage of C++11 constexpr is that
	// static field don't need to be initialized
	// separated from the class or struct.
	static constexpr const char* name		= "unknown";
	static constexpr size_t      size		= sizeof(T);
	static constexpr bool        isNumber	= false;
	static constexpr bool        isPointer	= ::isPointer<T>::value;
	static constexpr bool        isConst	= ::isConst<T>::value;		
};

// Macro for template specialization and type registration
#define REGISTER_TYPE_INFO(type, isNumberFlag) \
	template<> struct Typeinfo<type>{ \
		static constexpr const char* name		= #type;		\
		static constexpr size_t      size		= sizeof(type); \
		static constexpr bool        isNumber	= isNumberFlag;	\
		static constexpr bool        isPointer	= ::isPointer<type>::value; \
		static constexpr bool        isConst	= ::isConst<type>::value;	\
	}

// Template specilization example for type int 
template<> struct Typeinfo<int>{
	static constexpr const char* name		= "int";
	static constexpr size_t      size		= sizeof(int);
	static constexpr bool        isNumber	= true;
	static constexpr bool        isPointer	= ::isPointer<int>::value;
	static constexpr bool        isConst	= ::isConst<int>::value;
};

REGISTER_TYPE_INFO(bool,				false);
REGISTER_TYPE_INFO(char,				false);
REGISTER_TYPE_INFO(const char*,			true);
REGISTER_TYPE_INFO(const char,			false);
REGISTER_TYPE_INFO(short,				true);
REGISTER_TYPE_INFO(short*,				false);
REGISTER_TYPE_INFO(float,				true);
REGISTER_TYPE_INFO(float*,				false);
REGISTER_TYPE_INFO(const float&,		false);
REGISTER_TYPE_INFO(double,				true);
REGISTER_TYPE_INFO(double*,				false);
REGISTER_TYPE_INFO(const double&,		false);
REGISTER_TYPE_INFO(long,				true);
REGISTER_TYPE_INFO(std::string,			false);
REGISTER_TYPE_INFO(const std::string&,	false);

template<typename T>
void showTypeInfo(){
	std::cout << std::boolalpha;
	std::cout << std::setw(5) << "Type Info: name = " << std::setw(15) << Typeinfo<T>::name
			  << std::setw(5) << " ; bytes = "        << std::setw(4) << Typeinfo<T>::size
			  << std::setw(5) << " ; isNumber = "     << std::setw(5) << Typeinfo<T>::isNumber
			  << std::setw(5) << " ; isPointer = "    << std::setw(5) << Typeinfo<T>::isPointer
			  << std::setw(5) << " ; isConst = "      << std::setw(5) << Typeinfo<T>::isConst
			  << "\n";
}

int main(){
	// Allows writing bools as 'true' or 'false' instead of 0 or 1
	std::cout << std::boolalpha;
	// Confirm that the computation happens in compile-time 
	static_assert(Typeinfo<short>::isNumber == true, "Short type should be a number");
	static_assert(isPointer<short*>::value == true, "Short* should be a pointer");

	std::cout << "isPointerOLD<short*>::value  = " << isPointerOLD<short*>::value << "\n";
	std::cout << "isPointerOLD<short>::value   = " << isPointerOLD<short>::value << "\n";
	std::cout << "isPointerOLD<double>::value  = " << isPointerOLD<double>::value << "\n";
	std::cout << "isPointerOLD<double*>::value = " << isPointerOLD<double*>::value << "\n";

	
	std::cout << "isPointer<short*>::value  = " << isPointer<short*>::value << "\n";
	std::cout << "isPointer<short>::value   = " << isPointer<short>::value << "\n";
	std::cout << "isPointer<double>::value  = " << isPointer<double>::value << "\n";
	std::cout << "isPointer<double*>::value = " << isPointer<double*>::value << "\n";

	std::cout << "isPointer<short*>()()  = " << isPointer<short*>()() << "\n";
	std::cout << "isPointer<short>()()   = " << isPointer<short>()() << "\n";
	std::cout << "isPointer<double>()()  = " << isPointer<double>()() << "\n";
	std::cout << "isPointer<double*>()() = " << isPointer<double*>()() << "\n";		
	
	showTypeInfo<bool>();
	showTypeInfo<char>();
	showTypeInfo<std::string>();
	showTypeInfo<int>();
	showTypeInfo<short>();
	showTypeInfo<float>();
	showTypeInfo<double>();
	showTypeInfo<const char*>();
	showTypeInfo<float*>();
	showTypeInfo<double*>();
	showTypeInfo<const double&>();


	disp(Typeinfo<removePointer<double>::type>::name);
	disp(Typeinfo<removePointer<double*>::type>::name);
	disp(Typeinfo<removePointer<const char*>::type>::name);
	
	return 0;
}
