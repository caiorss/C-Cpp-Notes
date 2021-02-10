// File:   crtp1.cpp 
// Brief:  Demonstration of curious recurring template
// Author: Caio Rodrigues
//-------------------------------------------------------//
#include <iostream>

template<class Implementation>
class IntervalSummation{
public:
	// Get reference to implementation 
	Implementation& self(){
		return *static_cast<Implementation*>(this);
	}
	// Overload method 
	const Implementation& self() const {
		return *static_cast<Implementation const * const>(this);
	} 	
	double summation(int lower, int upper) const {
        double sum = 0;
        for(int i = lower; i <= upper; i++)
			sum += self().stepFn(i);
        return sum; 
	}
};

class SumOfSquares: public IntervalSummation<SumOfSquares>{
public:
	double stepFn(double x) const { return x * x; }
};

class SumOfCubes: public IntervalSummation<SumOfCubes>{
public:
	double stepFn(double x) const { return x * x * x; }
};

template<class T>
void clientCode(const IntervalSummation<T>& obj){
	std::cout << "Summation at [0, 15] = " << obj.summation(0, 15) << std::endl;
}

int main(){
	SumOfSquares sq;
	SumOfCubes   sc;
	std::cout << "Sum of squares in [0, 10] = " << sq.summation(0, 10) << std::endl;
	std::cout << "Sum of cubes   in [0, 10] = " << sc.summation(0, 10)   << std::endl;
	clientCode(sq);
	clientCode(sc);
	return 0;
}
