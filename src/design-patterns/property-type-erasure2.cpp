// Author: Caio Rodrigues
// Brief:  Properties with type-erasure and runtime discovery 
//-----------------------------------------------

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <iomanip>
#include <vector>

// Forward declarations
//-------------------------

template <typename T>
class TProperty;

class IProperty
{		
public:	
	virtual std::string           Name() const = 0;
	virtual std::type_info const& Type() const = 0 ;
	virtual void                  Print(std::ostream& os) const = 0;

	virtual ~IProperty() = default;
	
	// Note: Adding Non virtual methods does not causes break the base class ABI
	// or binary compatibility with derived classes (fragile-base class problem).
	template<typename T>
	T Get() const {
		if(this->Type() != typeid(T)) throw std::bad_cast();
		return static_cast<TProperty<T> const*>(this)->Get();
	}

	template<typename T>
	void Set(T const& value) {
		if(this->Type() != typeid(T)) throw std::bad_cast();
		static_cast<TProperty<T>*>(this)->Set(value);
	}

	template<typename T>
	TProperty<T>& As()
	{
		if(this->Type() != typeid(T)) throw std::bad_cast();
		return *static_cast<TProperty<T>*>(this);
	}

	template<typename T, typename Visitor>
	void Visit(Visitor& visitor)
	{
		if(this->Type() != typeid(T)) throw std::bad_cast();
		static_cast<TProperty<T>*>(this)->Visit(visitor);
	}

	// Make class printable 
	friend std::ostream& operator<<(std::ostream& os, IProperty const& rhs)
	{
		rhs.Print(os);
		return os;
	}
};

/** Class that encapsulate get/set properties 
 *  @tparam - Type default constructible, copiable and equality-comparable 
 */
template <typename T>
class TProperty: public IProperty
{
	std::string     m_name;	
	T               m_value;
	std::type_info const* m_tinfo;
public:
	TProperty(std::string name, T const& init = T{})
		: m_name(std::move(name))
		, m_value(init)
		, m_tinfo(&typeid(T))
	{ }

	~TProperty() = default;
	
	std::string Name() const 
	{
		return m_name;
	}
	
	const std::type_info& Type() const
	{
		return *m_tinfo;
	}
	T Get() const
	{
		return m_value;
	}
	TProperty<T>& Set(T const& value)
	{
		std::cerr << " [TRACE] Property [" << m_name << "] set to value = "
				  << value << std::endl;
		m_value = value;
		return *this;
	}

	void Print(std::ostream& os) const
	{
		os << " Property{ Name = " << std::quoted(m_name)
		   << " ; Value = " << m_value << " }";		
	}

	void PrintValue(std::ostream& os) const
	{
		os << m_value;
	}

	template<typename Visitor>
	void Visit(Visitor& visitor)
	{
		visitor.visit(m_value);
	}
};

template<typename T>
std::shared_ptr<IProperty>
make_property(std::string const& name, T const& init = T{})
{
	return std::make_shared<TProperty<T>>(name, init);
}

int main()
{
	std::vector<std::shared_ptr<IProperty>> plist;
	
	plist.push_back(make_property<int>("basis-points", 100));
	plist.push_back(make_property<double>("price", 5.6));
	plist.push_back(make_property<std::string>("product", ""));

	std::cout <<"\n ======= Experiment 1 =======" << std::endl;
	int i = 0; 
	for(const auto& p: plist)
		std::cout << " => p[" << i++ << "] = " << *p << "\n";

	std::cout <<"\n ======= Experiment 2 =======" << std::endl;
	TProperty<int>& p0 = plist[0]->As<int>();
	p0.Set(200);
	std::cout << " => p0.Name() = " << p0.Name() << " ; Value = " << p0.Get() << "\n";

	plist[0]->Set(80);
	
	std::cout << " => plist[0]->Name() = "  << plist[0]->Name()
			  <<  " plist[0]->Get<int>() = " << plist[0]->Get<int>()
			  << std::endl;
	
	return 0;
}
