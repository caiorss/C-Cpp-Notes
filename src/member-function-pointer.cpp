#include <iostream>
#include <string>
#include <iomanip>
#include <deque>
#include <map>
#include <cassert>

class Dummy {
private:
        std::string _name = "unnamed";
public:
        Dummy(){}
        Dummy(const std::string& name): _name(name){}
        ~Dummy() = default;
        std::string getName() {
                return "I am a dummy class named <" + _name + ">";
        }
        std::string getLocation() {
                return "Unknown location";
        }
        std::string operator()(){
                return "I am a function-object called: <" + _name + ">";
        }
        auto compute(double x, double y) -> double {
                return 4 * x + 5 * y;
        }	
};

class DummyB{
public:
        DummyB(){}
        ~DummyB() = default;
        std::string getName() {
                return "My name is DummyB";
        }
        std::string getLocation() {
                return "Location of dummyB location";
        }
        std::string operator()(){
                return "I am the class DummyB";
        }
};

// Create type synonym to any member function of Dummy class
// which takes no parameter and returns a string.
typedef std::string (Dummy::* pDummyMemFnStr)();

void invokeMemberFun(Dummy& obj, pDummyMemFnStr pMemfn){
        std::cout << " [1] Method invocation returned value: " << (obj.*pMemfn)() << "\n";
}

// Create type synonum with the new "using" C++11 syntax
using pDummyMemFnStrCPP11 = std::string (Dummy::*)();

auto invokeMemberFun2(Dummy& obj, pDummyMemFnStrCPP11 pMemfn) -> void {
        std::cout << " [2] Method invocation returned value: " << (obj.*pMemfn)() << "\n";
}

template<class T>
auto invokeMemberFun3(T& obj, std::string (T::* pMemfn)()) -> void{
        std::cout << " [3] Method invocation returned value: " << (obj.*pMemfn)() << "\n";
}

template<class T, class R, class ... Args>
auto invokeMemfn(R (T::* pMemfn) (Args ... args), T& obj, Args ... arglist) -> R{
        return (obj.*pMemfn)(arglist ...);
}

int main(){
        const auto nl = std::string("\n");
        const auto nl2 = std::string("\n\n");
        const std::string line = "--------------------------------------------------\n";

        std::cout << nl << "=== Experiment 1 ===============" << nl2;
        std::cout << line;
        // Pointer to member function to any member functions
        // (aka method) which takes no argument and returns a string
        // of signature: () => std::string
        //.....................................................
        std::string (Dummy::* pMemfn) () = nullptr;
        // std::string (Dummy::* pMemfn) ();
        if(pMemfn == nullptr)
                std::cerr << " [INFO] Pointer not initilialized yet." << nl;

        Dummy d("DUMMY");

        // Set the function pointer to member function getName().
        pMemfn = &Dummy::getName;
        // Invoke pointer to member function (aka pointer to method)
        std::cout << "Name     = " << (d.*pMemfn)() << nl2;
        assert((d.*pMemfn)() == "I am a dummy class named <DUMMY>");

        if(pMemfn != nullptr)
                std::cerr << " [INFO] Pointer initilialized OK." << nl;

        // Set pointer to Dummy::getLocation
        pMemfn = &Dummy::getLocation;
        std::cout << "Location = " << (d.*pMemfn)() << nl;
        // assert((d.*pMemfn)() == "I am a dummy class named <DUMMY>");	


        std::cout << nl << "=== Experiment 2 - Using typedef ===============" << nl;
        std::cout << line;
        pDummyMemFnStr pMemfn2 = nullptr;
        pMemfn2 = &Dummy::getName;
        std::cout << "d.getName() == " << (d.*pMemfn2)() << nl;
        assert((d.*pMemfn2)() == "I am a dummy class named <DUMMY>");	

        std::cout << nl << "=== Experiment 3 - Invoking member function with free function =" << nl;
        std::cout << line;
        // execute d.getName() 
        invokeMemberFun(d, &Dummy::getName);
        // execute d.getLocation() 
        invokeMemberFun(d, &Dummy::getLocation);
        // execute d() 
        invokeMemberFun(d, &Dummy::operator());

        std::cout << nl << "=== Experiment 4 - Invoking member function with free function C++11" << nl;
        std::cout << line;
        invokeMemberFun2(d, &Dummy::getName);
        invokeMemberFun2(d, &Dummy::getLocation);
        invokeMemberFun2(d, &Dummy::operator());

        std::cout << nl << "=== Experiment 5 - Pointer to member functions in STL deque collection" << nl;
        auto plist = std::deque<pDummyMemFnStrCPP11>();
        plist.push_back(&Dummy::getName);
        plist.push_back(&Dummy::getLocation);
        plist.push_back(&Dummy::operator());
        for(const auto& p: plist)
                std::cout << " (+) Calll returned = " << (d.*p)() << nl;

        std::cout << nl << "=== Experiment 6 - Pointer to member functions in map collection" << nl;
        auto dict = std::map<std::string, pDummyMemFnStrCPP11>();
        dict["getName"]     = &Dummy::getName;
        dict["getLocation"] = &Dummy::getLocation;
        dict["callme"]      = &Dummy::operator();
        for(const auto& kv: dict)
                std::cout << std::right << std::setw(20)  << "invoke(object, "
                                  << std::setw(15) << kv.first << ")"
                                  << " = " << (d.*(kv.second))() << nl;

        std::cout << nl << "=== Experiment 7 - Template " << nl;
        DummyB b;
        invokeMemberFun3(d, &Dummy::getName);
        invokeMemberFun3(b, &DummyB::getName);
        invokeMemberFun3(d, &Dummy::getLocation);
        invokeMemberFun3(b, &DummyB::getLocation);

        std::cout << nl << "=== Experiment 8 - Template " << nl;
        std::cout << line;
        std::cout << "d.getName()         = " << invokeMemfn(&Dummy::getName, d) << nl;
        std::cout << "d.compute(3.0, 4.0) = " << invokeMemfn(&Dummy::compute, d, 3.0, 4.0) << nl;

        return 0;
}
