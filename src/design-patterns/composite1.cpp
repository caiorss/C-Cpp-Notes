// Brief:  Composite Design Pattern
// Author: Caio Rodrigues
//----------------------------------------------
#include <iostream> 
#include <memory>
#include <vector>
#include <deque>

// Interface ->
class IGraphic{
public:
	virtual auto type() const -> const std::string  = 0;
	virtual auto draw() -> void = 0;
	virtual auto rotate(double) -> void = 0;
	virtual ~IGraphic() = default;
};

using cstring = const char*;
using GNode = std::shared_ptr<IGraphic>;

class Group: public IGraphic{
private:
	std::vector<GNode> _nodes;
	const std::string _id;
	static constexpr cstring _type = "Group";
public:
	Group(const std::string& id): _id(id) {
		std::cout << " [TRACE] Create group = " << id << "\n";
	}
	~Group(){
		std::cout << " [TRACE] Destroy group - id = << " << _id << "\n";
	}
	
	auto begin() const -> decltype(_nodes.begin()) {
		return _nodes.begin();
	}
	auto end() const -> decltype(_nodes.end()) {
		return _nodes.end();
	}
	
	auto clear() -> void {
		_nodes.clear();
	}
	auto size() -> size_t {
		return _nodes.size();
	}
	auto add(GNode n) -> void {
		std::cout << " [TRACE] id = " << _id
				  << "; Add object = " << n->type() << "\n";
		_nodes.push_back(n);
	}
	auto add(IGraphic* n) -> void {
		std::cout << " [TRACE] id = " << _id
				  << " ; Add object = " << n->type() << "\n";
		_nodes.push_back(std::shared_ptr<IGraphic>(n));
	}
	// Add stack-allocated object 
	auto addFromStack(IGraphic* n) -> void {
		std::cout << " [TRACE] id = " << _id
				  << " ; Add object = " << n->type() << "\n";
		// Dummy deleter to avoid core dump by avoiding deleting
		// stack-allocated object or non-owned pointer.
		auto s = std::shared_ptr<IGraphic>(n, [](IGraphic*){ return ; });
		_nodes.push_back(s);
	}	
	
	// Note: Template methods should always be in the header files
	template<class Node>
	auto addNew(const std::string& id) -> void {
		auto n = std::make_unique<Node>(id);
		std::cout << " [TRACE] id = " << _id
				  << " ; Add object = " << n->type() << "\n";    
		_nodes.push_back(std::move(n));
	}

	// ======> Interface IGraphic <<=========//
  
	auto type() const -> const std::string override {
		return _type;
	}    
	auto draw() -> void override {
		std::cout << " [TRACE] Draw group - id =  " << _id << "\n";
		for(const auto& obj: _nodes)
			obj->draw();
	}
	auto rotate(double angle) -> void override {
		std::cout << " [TRACE] Rotate group - id = " << _id << "\n";
		for(const auto& obj: _nodes)
			obj->rotate(angle);
	}    
};

class Line: public IGraphic {
private:
	static constexpr cstring _type = "Line";
	std::string _id;
public:
	Line(const std::string& id): _id{id} { }
	auto type() const -> const std::string override {
		return _type;
	} 
	auto draw() -> void override {
		std::cout << " [TRACE] Draw line - id = " << _id << "\n";
	} 
	auto rotate(double angle) -> void override {
		std::cout << " [TRACE] Rotate line ; id = " << _id 
				  << "; angle = " << angle << "\n";
	}  
};

class Triangle: public IGraphic {
private:
	static constexpr cstring _type = "Triangle";
	std::string _id;
public:
	Triangle(const std::string& id): _id{id} { }
	auto type() const -> const std::string override {
		return _type;
	} 
	auto draw() -> void override {
		std::cout << " [TRACE] Draw triangle - id = " << _id << "\n";
	} 
	auto rotate(double angle) -> void override {
		std::cout << " [TRACE] Rotate triangle"
				  << " id = " << _id
				  << " angle = " << angle << "\n";
	}  
};


// Count total number of elements 
auto countElements(const Group& group) -> size_t {
	size_t n = 0;
	for(const auto& g: group){
		if(g->type() == "Group"){
			auto p = static_cast<Group*>(g.get());
			n += countElements(*p);			
		} else{ ++n; }		
	}
	return n;
}


int main(){
	const char nl = '\n';
  
	std::cout << "=== Objects construction === " << nl;
  
	auto groupA = Group("groupA");
	groupA.add(new Triangle("triangleA1"));
	groupA.add(new Line("lineA1"));
	groupA.addNew<Line>("LineA2");

	auto groupB = std::make_shared<Group>("GroupB");
	groupB->add(new Triangle("triangleB1"));
	groupB->addNew<Triangle>("triangleB2");
	groupB->addNew<Line>("LineB1");
	groupB->addNew<Line>("LineB2");
	auto triangleB3 = Triangle("triangleB3");
	groupB->addFromStack(&triangleB3);
	groupA.add(groupB);

	std::cout << nl << "=== End of object construction === " << nl;
	std::cout << "Total of elements of groupA = " << countElements(groupA) << "\n";
	std::cout << "Total of elements of groupB = " << countElements(*groupB) << "\n";
	
	std::cout << nl << " [*] ==> Draw group B" << "\n";
	groupB->draw();

	std::cout << nl << " [*] ==> Rotate group B" << "\n";
	groupB->rotate(90);

	std::cout << nl << " [*] ==> Draw group A" << "\n";
	groupA.draw();

	std::cout << nl << " [*] ==> Rotate group A" << "\n";
	groupA.rotate(15);

	std::cout << nl << " [*] ==> Remove objects from group B" << "\n";
	groupB->clear();
	groupA.draw();

	std::cout << "=== End of Program ====" << "\n";
  
	return 0;
}
