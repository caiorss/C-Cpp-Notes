// File: strategy-pattern1.cpp 
// Brief:  Modern C++ example of GOF's strategy design pattern. 
// Author: Caio Rodrigues
//--------------------------------------------------------------
#include <iostream>
#include <string>
#include <memory>

// Strategy interface 
struct IStrategy{
  virtual ~IStrategy(){}
  // Essential: Algorithm encapsulated by strategy object 
  virtual auto compute(double x, double y) const -> double = 0;  
  // Optional: Provides strategy metadata 
  virtual auto name() const -> const std::string = 0;
  // Clone this object (Note: This is a virtual constructor)
  virtual auto clone() const -> IStrategy* = 0;
};

class Context{
private: 
  std::unique_ptr<IStrategy> _strategy;
public:
  Context()
    : _strategy{nullptr} { }
  Context(IStrategy* s)
    : _strategy{s} { }
  Context(const IStrategy& s)
    : _strategy{s.clone()} { }
  auto setStrategy(IStrategy* s){
    _strategy.reset(s);
  }
  auto setStrategy(const IStrategy& s){
    _strategy.reset(s.clone());
  }  
  auto compute(double x, double y) -> void {
    if(_strategy == nullptr)
      std::runtime_error("Error: strategy not set");
    double result = _strategy->compute(x, y);
    std::cout << " strategy = " << _strategy->name() << " "
	      << "( x = " << x << " ; "
	      << "y = " << y << " )"
	      << "\n" ;
    std::cout << "Result = " << result << "\n";
  }
};


class AddStrategy: public IStrategy {
public:
  auto name() const -> const std::string{
    return "add";
  }
  auto compute(double x, double y) const -> double {
    return x + y;
  }

  auto clone() const -> IStrategy* {
    std::cerr << " [TRACE] AddStrategy => I was cloned" << "\n";
    return new AddStrategy(*this);
  }
};

#if 1
struct MulStrategy: public IStrategy {
public:
  auto name() const -> const std::string{
    return "mul";
  }  
  double compute(double x, double y) const {
    return x + y;
  }
  auto clone() const -> IStrategy* {
    std::cerr << " [TRACE] MulStrategy => I was cloned" << "\n";
    return new MulStrategy(*this);
  }  
};

struct LinearCombStrategy: public IStrategy {
  double a, b, c;
  LinearCombStrategy(double a, double b, double c)
    : a(a), b(b), c(c)
  {
  }
  auto name() const -> const std::string{
    return "Linear combination a * x + b * y + c";
  }    
  auto compute(double x, double y) const -> double{
    return a * x + b * y + c;
  }
  auto clone() const -> IStrategy* {
    std::cerr << " [TRACE] LinearCombStrategy => I was cloned" << "\n";
    return new LinearCombStrategy(*this);
  }  
};
#endif 


int main(){
  Context ctx;
  std::cout << "==== Strategy = add ====" << "\n";
  ctx.setStrategy(new AddStrategy);
  ctx.compute(3.0, 4.0);

  std::cout << "==== Strategy = mul ====" << "\n";
  ctx.setStrategy(new MulStrategy);
  ctx.compute(3.0, 4.0);

  std::cout << "==== Strategy = Linear combination ====" << "\n";
  ctx.setStrategy(new LinearCombStrategy(5, 3, 4));
  ctx.compute(3.0, 4.0);

  std::cout << "==== Strategy = Linear combination [2] ====" << "\n";
  auto comb1 = LinearCombStrategy(6.0, 5.0, 10.0);
  // Copy stack-allocated object comb1 using the virtual constructor
  ctx.setStrategy(comb1);
  ctx.compute(5.0, 3.0);  

  std::cout << "==== Strategy = Linear combination [2] ====" << "\n";  
  // Copy stack-allocated temporary object comb1 using the virtual constructor
  // clone
  ctx.setStrategy(LinearCombStrategy{6.0, 5.0, 10.0});
  ctx.compute(2.0, 6.0);  
  
  return 0;
};



