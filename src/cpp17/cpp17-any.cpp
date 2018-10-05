#include <iostream>
#include <string>
#include <iomanip>
#include <ostream>

#include <any>

struct Point{
    double x;
    double y;
    Point(double x, double y): x(x), y(y) {}

    // Copy constructor
    Point(const Point& p){
        std::cerr << " -->> Copy constructor" << '\n';
        x = p.x;
        y = p.y;
    }   
};

std::ostream& operator<<(std::ostream& os, const Point& p){
    os << "Point(" << p.x << ", " << p.y << ") ";
    return os;
}

template<typename T>
auto printInfo(std::any x) -> void{
    std::cout << " x.type = " << x.type().name()
              << " ; value(x) = "
              << std::any_cast<T>(x)
              << '\n';  
}

int main(){
    // Print boolean as 'true', 'false', instead of 0 or 1
    std::cout << std::boolalpha;
    std::any x = 1;
    printInfo<int>(x);
    x = 10.233;
    printInfo<double>(x);
    x = 'k';
    printInfo<char>(x);
    x = "hello world";
    printInfo<const char*>(x);
    x = std::string("hello world");
    printInfo<std::string>(x);  
    x = Point(100.0, 20.0);
    printInfo<Point>(x);
    std::cout << "Has value: x.has_value() = " << x.has_value() << '\n';
    x.reset();                                                         
    std::cout << "Has value: x.has_value() = " << x.has_value() << '\n';                                                               
    std::cout << "Try casting " << std::endl;
    x = "testing type casting";
    try{
        std::any_cast<int>(x);
    } catch (const std::bad_any_cast& ex) {
        std::cerr << " >>> Exception: what = " << ex.what() << '\n'; 
    }
    std::cerr << " >>> End the program gracefully" << '\n'; 
    return 0;
}
