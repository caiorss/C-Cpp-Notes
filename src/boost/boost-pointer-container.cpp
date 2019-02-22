// Author: Caio Rodrigues
// Brief:  Demonstration and testing of boost pointer container.
//---------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include <boost/ptr_container/ptr_vector.hpp>

class Base{
public:
	static auto nextID() -> int {
		static int i = 0;
		return ++i;
	}		
	
	Base() = default;
	// Destructor of base class must always be virtual
	virtual ~Base() = default;

	virtual auto getID() const -> int = 0;

	virtual auto getType() const -> std::string = 0;

};


class DerivedA: public Base{	
public:
	const int m_id;
	
    DerivedA(): m_id(Base::nextID())  { }
    auto getType() const  -> std::string {
       return "DerivedA";
    }
	auto getID() const -> int {
		return m_id;
	}	
	~DerivedA(){
		std::cout << " [INFO] Class DerivedA deleted. => Object ID = "
				  << m_id << "\n";
	}
};


class DerivedB: public Base{
	const int m_id;
public:		
	DerivedB(): m_id(Base::nextID())  { }
	auto getType() const -> std::string {
      return "DerivedB";
	}
	auto getID() const -> int {
		return m_id;
	}		
	~DerivedB(){
		std::cout << " [INFO] Class DerivedB deleted. => ObjectID = "
				  << m_id  << "\n";
	}
};

	
void showType(Base const& obj)
{
	std::cout << "Object ID = " << obj.getID()
			  <<  " Class type = " << obj.getType()
			  << "\n";
}

int main(){

	std::cout << "\n === EXPERIMENT 0 ==============================" << "\n";
	std::vector<DerivedA> xsa;
	xsa.push_back(DerivedA());
	xsa.push_back(DerivedA());
	xsa.emplace_back();

	std::cout << "Run std::for_each" << "\n";
	std::for_each(xsa.begin(), xsa.end(), showType);	
	
	std::cout << "\n === EXPERIMENT 1 ==============================" << "\n";

	std::vector<std::shared_ptr<Base>> xs;
	xs.push_back(std::make_shared<DerivedA>());
	xs.push_back(std::make_shared<DerivedB>());
	xs.push_back(std::make_shared<DerivedA>());
	xs.push_back(std::make_shared<DerivedB>());

	std::cout << " <<INFO>> xs[0]  type " << xs[0]->getType()
			  << " ; id = " << xs[0]->getID() << "\n";

	std::cout << " <<INFO>> xs[2]  type " << xs[2]->getType()
			  << " ; id = " << xs[2]->getID() << "\n";

	std::for_each(xs.begin(), xs.end(),
				  [](auto const& pBase){
					  showType(*pBase);
				  });

	std::vector<int> identifiers1;
	std::transform(xs.begin(), xs.end(),
				   std::back_inserter(identifiers1),
				   [](auto pBase){ return pBase->getID(); });
		

	std::cout << "\n === EXPERIMENT 2 ==============================" << "\n";

	boost::ptr_vector<Base> ps;
	ps.push_back(new DerivedA);
	ps.push_back(new DerivedB());
	ps.push_back(new DerivedA());
	ps.push_back(new DerivedB);
	
	std::cout << " <<INFO>> ps[0]  type " << ps[0].getType() << " ; id = " << ps[0].getID() << "\n";
	std::cout << " <<INFO>> ps[1]  type " << ps[1].getType() << " ; id = " << ps[1].getID() << "\n";
	std::cout << " <<INFO>> ps[2]  type " << ps[2].getType() << " ; id = " << ps[2].getID() << "\n";

	std::vector<int> identifiers2;
	std::transform(xs.begin(), xs.end(),
				   std::back_inserter(identifiers2),
				   std::bind(&Base::getID, std::placeholders::_1));
	
	std::cout << "\n ==> Show objects before deleting last item " << "\n";
	std::for_each(ps.begin(), ps.end(), showType);

	std::cout << "\n ==> Show objects after deleting last item " << "\n";
	ps.pop_back();
	std::for_each(ps.begin(), ps.end(), showType);

	std::cout << " ============= END =================" << "\n";
	
	return 0;
}

