#include <iostream>
#include <string>
#include <ostream>

// std::invoke is provide by header functional
#include <functional> 

struct Dummy{
        double evalme(double x)	{
                std::cerr << __FILE__ << ":" << __LINE__ << " I was evaluated ; 2x = " << 2 *x << '\n';
                return 2 * x;
        }
        double operator()(double x){
                std::cerr << __FILE__ << ":" << __LINE__ << " Call function-operator << 4 * x = " << 4 * x << '\n';
                return 4 * x;
        }
};

double computeDouble(double x){
        std::cerr << __FILE__ << ":" << __LINE__ << " Computed double of 2x = " << 2 * x << '\n';
        return 2 * x;
}

int main(){
        std::invoke(computeDouble, 3.0);
        Dummy dummy;
        std::invoke(dummy, 3.0);
        std::invoke(Dummy(), 2.0);
        // Call method: .evalme indirectly 
        std::invoke(&Dummy::evalme, dummy, 3.0);
        return 0;
}
