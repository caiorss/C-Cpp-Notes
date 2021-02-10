/** File:    rpn-calculator.cpp
 * Author:  Caio Rodrigues - caiorss <DOT> rodrigues <AT> gmail <DOT> com
 * Brief:   Reverse Polish Notation Calculator
 * Description: A simple reverse polish notation command line calculator
 *              implemented in modern C++.
 ***************************************************************************/
#include <iostream>
#include <cmath>
#include <map>
#include <deque>
#include <sstream>
#include <ostream>
#include <string>
#include <string.h>
#include <memory>
#include <functional>
#include<limits>

enum class ASTtype{
	number,
	function
};

struct ASTNode{
  virtual auto getType() const -> ASTtype = 0;
  virtual ~ASTNode() = default;
};

struct ASTNum: public ASTNode{
  double num;
  ASTNum(double x): num(x){}
  auto getType() const -> ASTtype {
    return ASTtype::number;
  }
};

struct ASTFun: public ASTNode{
  std::string name;
  ASTFun(std::string name): name(name){}
  auto getType() const -> ASTtype {
    return ASTtype::function;
  }
};

using AST = std::deque<std::unique_ptr<ASTNode>>;

auto parseStreamToAST(std::istream& ss) -> AST {
  std::string token;
  std::stringstream p;
  double x;
  AST ast;
  while(!ss.eof()){
    ss >> token;
    // Clean p - stream and reset flag bits (p.clear())
    p.str(""); p.clear();
    // Try parse number or function
    p << token;
    p >> x;
    if(!p.fail() && p.eof()){
      // std::cout << "Number   = " << x << "\n";
      ast.emplace_back(new ASTNum(x));
      continue;
    }
    if(p.fail()){
      // std::cout << "Function = " << token << "\n";
      ast.emplace_back(new ASTFun(token));
      continue;
    }
    throw std::runtime_error("Error: bad input = " + token);
  }
  return ast;
}

void printAST(const AST& ast){
  for(const auto& a: ast){
    if(a->getType() == ASTtype::number){
      std::cout << "push operand num = "
		<< static_cast<ASTNum*>(a.get())->num << "\n";
    }
    if(a->getType() == ASTtype::function){
      std::cout << "execute function = "
		<< static_cast<ASTFun*>(a.get())->name << "\n";
    }
    continue;
  }
}

template<class T>
class Stack{
private:
  std::deque<T> _stack;
public:
  struct stack_empty_error: public std::exception{
    const char* what() const throw(){
      return " ==> Error: stack empty." ;
    }
  };
  void push(const T& t){
    _stack.push_back(t);
  }
  T pop(){
    if(_stack.empty())
      throw stack_empty_error();
    auto x = _stack.back();
    _stack.pop_back();
    return x;
  }
  T peek(){
    if(_stack.empty())
      throw stack_empty_error();
    return _stack.back();
  }
  size_t size(){  return _stack.size(); }
  bool   empty(){ return _stack.empty(); }
  void   clear(){ _stack.clear(); }
  void print(){
    std::cout << " stack: ";
    for(const auto& x: _stack)
      std::cout << " " << x ;
    std::cout << "\n";
  }
};

/** Simple Reverse Polish Notation Evaluator */
class RPNEvaluator
{
private:
  Stack<double> _stack;
  std::map<std::string, std::function<void ()>> _functions;
public:
  using UnaryFun  = std::function<double (double)>;
  using BinaryFun = std::function<double (double, double)>;
  using CmdFun    = std::function<void ()>;

  struct evalutor_error: public std::exception{
    const std::string text;
    evalutor_error(const std::string& text): text(text){}
    const char* what() const throw(){
      return  text.c_str();
    }
  };
	
  RPNEvaluator()
  {
    // Fundamental operators
    this->_functions["+"] =
      [this](){
	double a = this->pop();
	double b = this->pop();
	this->push(b + a);
      };
    this->_functions["-"] =
      [this](){
	double a = this->pop();
	double b = this->pop();
	this->push(b - a);
      };
    this->_functions["*"] =
      [this](){
	double a = this->pop();
	double b = this->pop();
	this->push(b * a);
      };
    this->_functions["/"] =
      [this](){
	double a = this->pop();
	double b = this->pop();
	this->push(b / a);
      };
    this->_functions["dup"] =
      [this](){
	this->push(this->peek());
      };
    this->_functions["drop"] =
      [this](){
	this->pop();
      };
    this->_functions["swap"] =
      [this](){
	double a = this->pop();
	double b = this->pop();
	this->push(a);
	this->push(b);
      };
    // Fundamental transcendental functions
    this->addFunction("abs", abs);
    this->addFunction("sqrt", sqrt);
    this->addFunction("cbrt", cbrt);
    this->addFunction("exp", exp);
    this->addFunction("log", log);
    this->addFunction("log10", log10);
    this->addFunction("sin", sin);
    this->addFunction("cos", cos);
    this->addFunction("tan", tan);
    this->_functions["inv"] =
      [this](){
	this->push(1.0 / this->pop());
      };
    // Turn stack value into percent
    this->_functions["pct"] =
      [this](){
	this->push(this->pop() / 100.0);
      };
    // Useful binary functions
    this->addBinaryFunction("hypot", hypot);

    // Math constants
    this->addConstant("M_PI", M_PI);
    this->addConstant("M_E", M_E);
    this->addConstant("M_LN2", M_LN2);
    this->addConstant("M_LN10", M_LN10);
    // Earth's gravity
    this->addConstant("M_G", 9.81);
  }
  auto addConstant(const std::string& name, double value) -> void{
    _functions[name] = [this, value](){
			 this->push(value);
		       };
  }
  auto addGenFunction(const std::string& name, CmdFun fun) -> void
  {
    _functions[name] = fun;
  }
  auto addFunction(const std::string& name, UnaryFun fun) -> void
  {
    _functions[name] = [this, fun](){
			 this->push(fun(this->pop()));
		       };
  }
  auto addBinaryFunction(const std::string& name, BinaryFun fun) -> void
  {
    _functions[name] =
      [this, name, fun](){
	//std::cerr << " Running function = " << name << std::endl;
	this->push(fun(this->pop(), this->pop())) ;
      };
  }
  // Composition and delegation
  auto push(double x) -> void {
    _stack.push(x);
  }
  auto pop() -> double {
    if(_stack.empty())
      throw RPNEvaluator::evalutor_error("Error: attemp to pop fom empty stack.");
    return _stack.pop();
  }
  auto clear() -> void {
    _stack.clear();
  }
  auto show() -> void {
    _stack.print();
  }
  auto peek() -> double {
    if(_stack.empty())
      throw RPNEvaluator::evalutor_error("Error: attemp to peek value fom empty stack.");
    return _stack.peek();
  }
  auto evaluate(const AST& ast) -> void
  {
    for(const auto& it : ast){
      if(it->getType() == ASTtype::number){
	_stack.push(static_cast<ASTNum*>(it.get())->num);
	continue;
      }
      if(it->getType() == ASTtype::function){
	std::string name = static_cast<ASTFun*>(it.get())->name;
	auto p = _functions.find(name);
	if(p != _functions.end()){
	  // std::cerr << " [INFO] found function = " << name << std::endl;
	  //_stack.print();
	  p->second();
	}
	else
	  throw evalutor_error("Error: invalid function <" + name + ">");
      }
    }
    //return this->peek();
  }
  auto evaluate(const std::string& expr) -> void{
    if(expr == "")
      return;
    auto ss = std::stringstream(expr);
    AST ast = parseStreamToAST(ss);
    return this->evaluate(ast);
  }
};

auto main() -> int {
  RPNEvaluator eval{};
  std::string line;

  eval.addGenFunction("clear",
		      [&](){ eval.clear();
		      });
  eval.addGenFunction("quit",
		      [&](){
			std::cout << " Exiting REPL OK." << "\n";
			exit(0);
		      });
  while(true){
    std::cout << " EXPR+> ";
    std::getline(std::cin, line);
    if(std::cin.fail()){
      // Reset error flags
      std::cin.clear();
    }
    try {
      eval.evaluate(line);
      //std::cout << " => " << eval.evaluate(line) << "\n";
      eval.show();
    } catch (const RPNEvaluator::evalutor_error& ex){
      std::cerr << " " << ex.what() << "\n";
      eval.show();
    }
  }

  return 0;
}
