#include <iostream>
#include <variant> // C++17
#include <string>
#include <ostream>
#include <deque>
#include <vector>
#include <iomanip>

template <class T>
auto display(const std::string& name, const T& t) -> void;

// Pattern matching using constexpr => May be the more performant way 
template<class T>
auto identifyAndPrint(const T& v) -> void;

struct VisitorOperation{
        auto operator()(int num) -> void {
                std::cout << "type = int     => value = " << num << "\n";
        }
        auto operator()(double num) -> void {
                std::cout << "type = double  => value = " << num << "\n";
        }
        auto operator()(const std::string& s){
                std::cout << "type = string  => value = " << s << "\n";
        }
};

int main(){
        // using <1>, <2>, ... <n> => Only available at C++17
        using std::cout, std::endl, std::cerr;
        auto nl = "\n";
        std::cout << std::boolalpha;
        cout << "========== Test 1 ==================" << nl;	

        // std::variant<int, double, std::string> somevar;
        auto x = std::variant<int, double, std::string>();
        x = 100;
        std::cout << "variant has int    = " << std::holds_alternative<int>(x) << nl;
        std::cout << "variant has double = " << std::holds_alternative<double>(x) << nl;
        std::cout << "variant has string = " << std::holds_alternative<std::string>(x) << nl;
        display("x", x);
        std::cout << "-------------------" << nl;
        x = 204.45;
        std::cout << "variant has double = " << std::holds_alternative<double>(x) << nl;	
        display("x", x);
        std::cout << "-------------------" << nl;
        x = "std::variant is awesome!";
        std::cout << "variant has string = " << std::holds_alternative<std::string>(x) << nl;
        display("x", x);

        cout << "========== Test 2 ==================" << nl;
        try{ 
                // Try to get int 
                int m = std::get<int>(x);
                std::cout << "m = " << m << "\n";
        } catch(const std::bad_variant_access& ex){
                std::cerr << "Error: Failed to extract int." << nl;
        }
        try{ 
                // Try to get string 
                auto s = std::get<std::string>(x);
                std::cout << "s = " << s << nl;
        } catch(const std::bad_variant_access& ex){
                std::cerr << "Error: Failed to extract string." << nl;
        }

        cout << "========== Test 3 ==================" << nl;
        x = -100;
        std::visit([](auto&& p){
                                   std::cout << "x = " << p << '\n';
                           }, x);
        x = 20.52;
        std::visit([](auto&& p){
                                   std::cout << "x = " << p << '\n';
                           }, x);

        x = "<hello world std::variant>";
        std::visit([](auto&& p){
                                   std::cout << "x = " << p << '\n';
                           }, x);

        cout << "========== Test 4 ==================" << nl;
        // auto + uniform initialization 
        auto xs = std::deque<std::variant<int, double, std::string>>{10.0, 20, 5, "hello", 10, "world"};
        for(const auto& e: xs){
                identifyAndPrint(e);
        }
        cout << "========== Test 5 ==================" << nl;
        for(const auto& e: xs){
                std::visit(VisitorOperation(), e);
        }	
        return 0;
}

// It works in a similar fashion to functional languages with
// pattern matching such as Haskell, Scala, OCaml and so on.
// std::variant is also a type-safe alternative to old C-unions.
template <class T>
auto display(const std::string& name, const T& t) -> void {
        auto nl = "\n";
        // Boost.Variant uses boost::get<TYPE>(&t), now changed to std::get_if
        if(auto n = std::get_if<int>(&t)){
                std::cout << " = " << *n << nl;
                return; // Early return 
        }
        if(auto d = std::get_if<double>(&t)){
                std::cout << name << " = " << *d << nl;
                return;
        }	
        if(auto s = std::get_if<std::string>(&t)){
                std::cout << name << " = " << *s << nl;
                return;
        }
        std::cout << "<UNKNOWN>" << std::endl;
}

template<class T>
auto identifyAndPrint(const T& v) -> void{
        std::visit([](auto&& a){
            using C = std::decay_t<decltype(a)>;
            if constexpr(std::is_same_v<C, int>){
                            std::cout << "Type is int => value = " << a << "\n";
                            return;
            }
            if constexpr(std::is_same_v<C, double>){
                            std::cout << "Type is double => value = " << a << "\n";
                            return;
            }
            if constexpr(std::is_same_v<C, std::string>){
                            std::cout << "Type is string => value = " << a << "\n";
                            return;
            }
            std::cout << "Type is unknown" << "\n";
                           }, v);
} // End of func. identifyAndPrint() ---//
