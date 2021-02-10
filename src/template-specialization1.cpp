#include <iostream>
#include <iomanip>    // Stream manipulator std::fixed, std::setw ... 
#include <vector>
#include <cmath>      // sin, cos, tan, exp ... M_PI, M_E ...
#include <functional> // std::function 

// ============= Example 1 ===============================//

// Check whether type is float point 
template<class T>
auto isFPNumber() -> bool {
        return false;
}
// Template specialization of isFPNumber for type float
template<> auto isFPNumber<float>() -> bool {
        return true;
}
// Template specialization of isFPNumber for type double 
template<> auto isFPNumber<double>() -> bool {
        return true;
}

// ============= Example 2 - Template specialization for runtime type identification ====//
// Note: this technique can be used for implemeting custom C++ reflection 

// Return name of a given type 
template<class Type>
auto TypeName() -> const char* { return "unknown"; }

#define REGISTER_TYPE(type)  template<> \
        auto TypeName<type>() -> const char* { return #type; } 

// Specialization for int type 
template<>
auto TypeName<int>() -> const char* { return "int"; }

// Automate boilerplate code using macros.
REGISTER_TYPE(bool);
REGISTER_TYPE(std::string);
REGISTER_TYPE(const char*);
REGISTER_TYPE(float);
REGISTER_TYPE(double);
REGISTER_TYPE(long);
REGISTER_TYPE(unsigned);
REGISTER_TYPE(char);
REGISTER_TYPE(long long);

// ============= Example 3 - Template with int argument specialization ===//
template<int>
const char* getNumberName(){
        return "I down't known";
}
template<> const char* getNumberName<0>(){ return "zero"; }
template<> const char* getNumberName<1>(){ return "one"; }
template<> const char* getNumberName<2>(){ return "two"; }
template<> const char* getNumberName<3>(){ return "three"; }

// ============= Example 4 - Template with bool argument specialization ====//
template<bool>
struct boolTemplate;

template<> struct boolTemplate<false>{
        static auto getName() -> const char* { return "false"; }
};
template<> struct boolTemplate<true>{
        static auto getName() -> const char* { return "true"; }
};

// ============= Example 5 - Check whether types are equal ====//
// Partial template specialization 

template<class A, class B>
struct type_equal{
        static bool get(){ return false; }
        enum { value = 0 };
};

// Partial specialisation
template<class A>
struct type_equal<A, A>{
        static bool get(){ return true; }
        enum { value = 1};
};

int main(){
        const char nl = '\n';
        std::cout << std::boolalpha;

        std::cout << nl << "EXPERIMENT 1 - Check whether type is float pointer" << nl;
        std::cout << "--------------------------------------------" << nl;	
        std::cout << "is float point type<int>    ? = " << isFPNumber<int>() << nl;
        std::cout << "is float point type<char>   ? = " << isFPNumber<char>() << nl;
        std::cout << "is float point type<float>  ? = " << isFPNumber<float>() << nl;
        std::cout << "is float point type<double> ? = " << isFPNumber<float>() << nl;

        std::cout << nl << "EXPERIMENT 2 - Type introspection" << nl;
        std::cout << "--------------------------------------------" << nl;	
        std::cout << "type = " << TypeName<int>() << nl;
        std::cout << "type = " << TypeName<char>() << nl;
        std::cout << "type = " << TypeName<float>() << nl;
        std::cout << "type = " << TypeName<const char*>() << nl;
        std::cout << "type = " << TypeName<std::string>() << nl;	

        std::cout << nl << "EXPERIMENT 3 - Templates with integers as arguments" << nl;
        std::cout << "--------------------------------------------" << nl;
        std::cout << "getNumberName<0>() = " << getNumberName<0>() << nl;
        std::cout << "getNumberName<1>() = " << getNumberName<1>() << nl;
        std::cout << "getNumberName<2>() = " << getNumberName<2>() << nl;
        std::cout << "getNumberName<10>() = " << getNumberName<10>() << nl;
        std::cout << "getNumberName<14>() = " << getNumberName<14>() << nl;

        std::cout << nl << "EXPERIMENT 4 - Templates with bool as arguments" << nl;
        std::cout << "--------------------------------------------" << nl;	
        std::cout << "boolTemplate<false>::getName>()  = " << boolTemplate<false>::getName() << nl;
        std::cout << "boolTemplate<true>::getName>()   = " << boolTemplate<true>::getName() << nl;

        std::cout << nl << "Check whether types are equal" << nl;
        std::cout << "type_equal<int, char>::get()       = "  << type_equal<int, char>::get() << nl;	
        std::cout << "type_equal<char, double>::get()    = "  << type_equal<char, double>::get() << nl;
        std::cout << "type_equal<double, double>::get()  = "  << type_equal<double, double>::get() << nl;
        std::cout << "type_equal<int, int>::get()        = "  << type_equal<int, int>::get() << nl;

        if(type_equal<int, double>::value)
                std::cout << "[1] Types are equal\n";
        else
                std::cout << "[1] Types are not equal\n";

        if(type_equal<double, double>::value)
                std::cout << "[2] Types are equal\n";
        else
                std::cout << "[2] Types are not equal\n";


        return 0;
}
