#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#define DBG_DISP(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  std::endl

// === file: Product.hpp - interface ====================//

class Product{
private:
	int          m_id;
	std::string  m_name;
	double       m_price;
public:
	Product();
	Product(int id, std::string const& name, double price);
	int         Id()    const;
	std::string Name()  const;
	double      Price() const;
	void showProduct()  const;
	
	friend
	auto operator<<(std::ostream& os, Product const& prod) -> std::ostream&;

	friend
	auto operator>>(std::istream& is, Product& prod) -> std::istream&;	
};

// === file: main.cpp ==================================//

int main(){
	Product p1{200, "Arabica Coffee", 4.50};	
	Product p2{300, "Orange 1kg ", 10.0};
	Product p3{126, "XYWZ soft drink", 15.60};	

	std::puts(" >>> EXPERIMENT 1 == Print Products ===========");
	std::cout << "Product p1 = {" << p1 << " }" << std::endl;
	std::cout << "Product p2 = {" << p2 << " }" << std::endl;
	std::cout << "Product p3 = {" << p3 << " }" << std::endl;

	std::puts(" >>> EXPERIMENT 2 == Print Products to memory stream (memory file) ====");
	std::stringstream ss;
	ss << p1 << "\n" << p2 << "\n" << p3;
	std::cout << "ss = \n" << ss.str() << std::endl;

	std::puts(" >>> EXPERIMENT 3 == Read products from memory stream ====");
	Product pr;
	ss >> pr;
	std::cout << " pr1 = " << pr << std::endl;
	ss >> pr;
	std::cout << " pr2 = " << pr << std::endl;
	ss >> pr;
	std::cout << " pr3 = " << pr << std::endl;

	std::puts(" >>> EXPERIMENT 4 == Read products from console ====");
	Product prod;
	while(!std::cin.eof()){
		std::cout << "Enter product: ";
		std::cin >> prod;
		std::cout << " prod = " << prod << " ; "
				  << " price = " << prod.Price()
				  << std::endl;
	}		
	
	return 0;
}

// ==== file: Product.cpp - Implementation ===============//

Product::Product(int id, std::string const& name, double price):
	m_id(id), m_name(name), m_price(price)
{
}

Product::Product(): Product(-1, "unnamed", 0.0)
{   
}

void Product::showProduct() const
{
	std::cout << "Product{ "
			  << "; id = " << m_id 
			  << " ; name = " << m_name
			  << " ; price = " << m_price
			  << " }\n" ;				  
}

int Product::Id() const
{
	return m_price;
}

std::string Product::Name() const
{
	return m_name;
}

double Product::Price() const
{
	return m_price;
}

auto operator<<(std::ostream& os, Product const& prod) -> std::ostream&
{
	os << " " << prod.m_id
	   << " " << std::quoted(prod.m_name)
   	   << " " << prod.m_price;	   
	return os;	
}

auto operator>>(std::istream& is, Product& prod) -> std::istream&
{
	return is >> prod.m_id >> std::quoted(prod.m_name) >> prod.m_price;	
}
