#include <iostream>   // cout, cin, endl
#include <functional> // Import std::function, bind, placeholders, _1, _2, _3 ... 
#include <iomanip>    // 
#include <cmath>      // sin, cos, tan, sqrt ... 
#include <vector>     // std::vector<X>

/*  Debug macro to print expressions - disp(x * 10 + 5) will print 
 *  on cerr (standard error output stream) the line: 
 *  "line = 105 file = test.cpp ; x * 10 + 5 = 25"
 */
#define disp(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
    << " ; " <<  #expr << " = " << (expr)  <<  std::endl
#define dbgloc(msg)  std::cerr << __FILE__ << ":" << __LINE__ << ":" << msg 
#define dbgline __FILE__ << ":" << __LINE__ << ":" <<
#define dbgtrace(msg) std::cout << __FILE__ << ":" << __LINE__ << ": TRACING " << msg << "\n";

// type Mfun = double => double 
using Mfun = std::function<double (double)>;
using NumVector = std::vector<double>;

// ========== Prototypes =============//
NumVector mapVector(NumVector& xs, Mfun fn);
NumVector makeVector(int size, std::function<double (int i)>);
void      tabulate(double start, double stop, double step, Mfun fn);
double    vectorLength(double x, double y, double z);
double    sum(double x, double y);

// Function object
struct FunctionObject{
    double x;
    double y;

    FunctionObject(double x, double y): x(x), y(y) {};

    double operator ()(double a){   
       return a * a;
    }   
    double operator ()(double a, double b, double c){
        return a * x + b * y + c / (x + y);
    }
    double method1(double a){
        return this->x * a  + this->y / a;
    }
    double method2(double a, double b, double c){
        return  c * (a / x + b / y);
    }       
};

int main(){
    using std::cout;
    using std::endl;
    using namespace std::placeholders; // Import placeholders, _1, _2, _3 ... 

    cout << "======== Test 1 ========" << endl;

    // Verbose signature 
    std::function<double (double)> sum10 = std::bind(sum, 10, _1);
    disp(sum10(2.0));
    disp(sum10(4.5));
    disp(sum10(25.0));

    cout << "======== Test 2 ========" << endl;

    // Fix y = 10.0 and z = 25.0 - set x as variable 
    Mfun vectorLenAsFunctionOfX = std::bind(vectorLength, _1, 10.0, 25.0);
    disp(vectorLenAsFunctionOfX(4.0));
    disp(std::bind(vectorLength, _1, 10.0, 25.0)(4.0));
    disp(vectorLenAsFunctionOfX(10.0));
    disp(std::bind(vectorLength, _1, 10.0, 25.0)(10.0));
    cout << "Tabulating - vectorLenAsFunctionOfX" << endl;
    tabulate(1.0, 5.0, 1.0, vectorLenAsFunctionOfX);

    cout << "======== Test 3 ========" << endl;

    // Fix x = 10.0, z = 25.0 - set y as variable
    auto vectorLenAsFunctionOfY = std::bind(vectorLength, 10.0, _1, 25.0);
    disp(vectorLenAsFunctionOfY(14.0));
    disp(vectorLenAsFunctionOfY(20.0));
    cout << "Tabulating - vectorLenAsFunctionOfY" << endl;
    tabulate(1.0, 5.0, 1.0, vectorLenAsFunctionOfY);    

    cout << "======== Test 4 ========" << endl;

    // Note: auto could be used here to simplify the singature.
    // It is the same as writing:
    // 
    //  vectorLenAsFunctionOfYZ =
    //    [&](double y, double z){ return vectorLength(10.0, y, z)};
    std::function<double (double, double)> vectorLenAsFunctionOfYZ =
        std::bind(vectorLength, 10.0, _1, _2);

    disp(vectorLenAsFunctionOfYZ(3.0, 6.0));
    disp(vectorLenAsFunctionOfYZ(15.0, 26.0));

    cout << "======== Test 5 ========" << endl;
    FunctionObject fobj(6.0, 8.0);
    disp(fobj(4.0));
    disp(fobj(5.0));
    disp(fobj(10.0));

    disp(fobj(4.0, 10.0, 5.0));
    disp(fobj(6.0, 8.0,  9.0));
    cout << "Running: tabulate(0.0, 5.0, 1.0, fobj)" << endl;
    tabulate(0.0, 5.0, 1.0, fobj);

    cout << "Turning class member function into lambda function " << endl;
    cout << " Note: it is not possible (0.0, 5.0, 1.0, fobj.method1)" << endl;

    // Equivalent to:: method1LambdaA = [&fobj](double x){ return fobj.method(x); };
    Mfun method1LambdaA = std::bind(&FunctionObject::method1, &fobj, _1);
    disp(fobj.method1(10.0));
    disp(method1LambdaA(10.0));
    disp(fobj.method1(20.0));
    disp(method1LambdaA(20.0));
    disp(fobj.method1(30.0));
    disp(method1LambdaA(30.0));

    cout << "Tabulating method1LambdaA" << endl;
    tabulate(0.0, 5.0, 1.0, method1LambdaA);
    cout << "Tabulating method1LambdaA using direct lambda" << endl;
    tabulate(0.0, 5.0, 1.0, std::bind(&FunctionObject::method1, &fobj, _1));

    cout << "======== Test 6 ========" << endl;

    // set parameters a = <variable>, b = 10.0 , c = 20.0
    auto method2LambdaAsFnOfA =
        std::bind(&FunctionObject::method2, &fobj, _1, 10.0, 20.0);

    disp(method2LambdaAsFnOfA(5.0));
    disp(method2LambdaAsFnOfA(6.0));
    disp(method2LambdaAsFnOfA(10.0));

    // set parameters a = <var>, b = 25.0> , c = <var>
    auto method2LambdaAsFnOfAC =
        std::bind(&FunctionObject::method2, &fobj, _1, 10.0, _2);
    disp(fobj.method2(3.0, 10.0, 4.0));
    disp(method2LambdaAsFnOfAC(3.0, 4.0));
    disp(fobj.method2(15.0, 10.0, 14.0));
    disp(method2LambdaAsFnOfAC(15.0, 14.0));


}
 //////////////// Prototypes Implementations ///////////////////////////

NumVector mapVector(NumVector& xs, Mfun fn){
    NumVector ys(xs.size());
    int n = xs.size();
    for(int i =0; i < n; i++){
        ys[i] = fn(xs[i]);
    }
    return ys;
}

NumVector makeVector(int size, std::function<double (int i)> fn){
    NumVector ys(size);
    for(int i =0; i < size; i++){
        ys[i] = fn(i);
    }
    return ys;  
}
void tabulate(double start, double stop, double step, Mfun fn){
    double x = start;
    while(x <= stop){
        std::cout << std::fixed
                  << std::setw(10) << std::setprecision(3) << x
                  << std::setw(10) << std::setprecision(3) << fn(x)
                  << "\n";
        x += step;
    }
}
double vectorLength(double x, double y, double z){
    return sqrt(x * x + y * y + z * z);
}
double sum(double x, double y){
    return x + y;
}
