// File:   interface-class.cpp 
// Brief:  Demonstration of interface class pattern/idiom. 
// Author: Caio Rodrigues
//----------------------------------------------------------

#include <iostream> 
#include <string>
#include <vector>
#include <deque>
#include <ostream>
#include <initializer_list>
#include <functional>
#include <memory> // Smart pointers

// Interface Stack. (Should be placed in the header file.) 
class IStack{
public:
	virtual ~IStack() = default;
	virtual int    size()         const = 0;
	virtual void   push(double x)       = 0;
	virtual double pop()                = 0;
	virtual double peek()         const = 0;
};

// Implementation of interface stack
// Using vector as internal representation 
class StackVector: public IStack{
public:
	StackVector(){}
	StackVector(const std::initializer_list<double>& xs){
		_stack.insert(_stack.begin(), xs.begin(), xs.end());
	}
	int size() const {
		return _stack.size();
	}	
	void push(double x){
		_stack.push_back(x);
	}   
	double pop(){
		if(this->size() == 0)
			throw std::runtime_error("Error: stack is empty");
		double top = _stack.back();
		_stack.pop_back();
		return top;
	}
	double peek() const {
		if(this->size() == 0)
			throw std::runtime_error("Error: stack is empty");		
		return _stack.back();
	}
private:
	std::vector<double> _stack{};
};

class StackDeque: public IStack{
public:
	StackDeque(){}
	StackDeque(const std::initializer_list<double>& xs){
		_stack.insert(_stack.begin(), xs.begin(), xs.end());
	}	
	int size() const {
		return _stack.size();
	}	
	void push(double x){
		_stack.push_back(x);
	}   
	double pop(){
		if(this->size() == 0)
			throw std::runtime_error("Error: stack is empty");
		double top = _stack.back();
		_stack.pop_back();
		return top;
	}
	double peek() const {
		if(this->size() == 0)
			throw std::runtime_error("Error: stack is empty");		
		return _stack.back();
	}
private:
	std::deque<double> _stack{};
};

auto stack_sum(IStack& s) -> double{
	//std::cerr << " ==> stack_sum for references" << std::endl;
	double sum = 0.0;
	if(s.size() == 0)
		return sum;
	while(s.size() != 0)
		sum += s.pop();
	return sum;
}

auto stack_sum(IStack* s) -> double{
	// std::cerr << " ==> stack_sum for pointers" << std::endl;
	double sum = 0.0;
	if(s->size() == 0)
		return sum;
	while(s->size() != 0)
		sum += s->pop();
	return sum;
}

int main(){
	StackVector sv = {1.0, 2.0, 3.0, 5.0, 6.0};
	StackDeque  sd = {1.0, 2.0, 3.0, 5.0, 6.0};
	// The same client code works with any implementation of the interface. 
	std::cout << "stack_sum(sv) = " << stack_sum(sv) << std::endl;
	std::cout << "stack_sum(sd) = " << stack_sum(sd) << std::endl;

	IStack* spointer = nullptr;
	StackVector sv2 = {1.0, 2.0, 3.0, 5.0, 6.0};
	StackDeque  sd2 = {1.0, 2.0, 3.0, 5.0, 6.0};
	spointer = &sv2;
	std::cout << "stack_sum(spointer) = " << stack_sum(spointer) << std::endl;
	spointer = &sd2;
	std::cout << "stack_sum(spointer) = " << stack_sum(spointer) << std::endl;

	auto sptr = std::unique_ptr<IStack, std::function<void (IStack*)>>{
		nullptr,
		// Custom deleter 
		[](IStack* p){
			std::cerr << " ==== Stack deleted OK" << std::endl ;
			delete p;
		}
	};
	sptr.reset(new StackVector());
	sptr->push(10);
	sptr->push(25.0);
	sptr->push(20.0);
	std::cout << "stack_sum(sptr) = " << stack_sum(*sptr) << std::endl;
	sptr.reset(new StackDeque());
	sptr->push(10);
	sptr->push(25.0);
	sptr->push(20.0);
	std::cout << "stack_sum(sptr) = " << stack_sum(*sptr) << std::endl;
	return 0;
}

