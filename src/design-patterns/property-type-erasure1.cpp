// Author: Caio Rodrigues
// Brief:  Property class without type erasure. 
//-----------------------------------------------

#include <iostream>
#include <string>
#include <functional>
#include <iomanip>

/** Class that encapsulate get/set properties 
 *  @tparam - Type default constructible, copiable and equality-comparable 
 */
template <typename T>
class TProperty
{
	std::string    m_name;	
	T              m_value;
	std::type_info const& m_tinfo;
public:
	TProperty(std::string name, T const& init = T{})
		: m_name(std::move(name))
		, m_value(init)
		, m_tinfo(typeid(T))
	{ }

	std::string Name() const 
	{
		return m_name;
	}
	
	const std::type_info& Type()
	{
		return m_tinfo;
	}
	TProperty<T> Get() const
	{
		return m_value;
	}
	TProperty<T>& Set(T const& value)
	{
		std::cerr << " [TRACE] Property [" << m_name << "] set to value = "
				  << value << std::endl;
		m_value = value;
	}
	
	friend std::ostream& operator<<(std::ostream& os, TProperty<T> const& rhs)
	{
		return os << " Property{ Name = " << std::quoted(rhs.m_name)
				  << " ; Value = " << rhs.m_value << " }";
	}
};

int main()
{
	TProperty<int>         p1("count", 100);
	TProperty<double>      p2("range", 5.6);
	TProperty<std::string> p3("name", "Box");

	std::cout << "p1 = " << p1 << "\n";
	std::cout << "p2 = " << p2 << "\n";
	std::cout << "p3 = " << p3 << "\n";
	
	return 0;
}
