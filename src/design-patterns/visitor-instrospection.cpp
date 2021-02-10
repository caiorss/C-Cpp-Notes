// File:   visitor-instrospection.cpp 
// Brief:  Visitor design pattern for class instrospection.
// Tags:   template metaprogramming reflection oop visitor design pattern 
// Author: Caio Rodrigues
//---------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include <istream>
#include <fstream>
#include <iomanip>

/** Print class information such as name and fields to stdout. */
struct DescriptionVisitor{
	using cstring = const char*;

	template<class Described>
	void visit(Described& desc){
		desc.describe(*this);
	}
	void name(const std::string& className){
		std::cout << "Class name = " << className << "\n";
	}
 	void field(int& value, cstring name){
		std::cout << " Field { name = " << name
				  << " ; type = int "
				  << " ; value = " << value << " }\n";
	}
	void field(long& value, cstring name){
		std::cout << " Field { name = " << name
				  << " ; type = long "
				  << " ; value = " << value << " }\n";	
	}
  	void field(double& value, cstring name){
		std::cout << " Field { name = " << name
				  << " ; type = double "
				  << " ; value = " << value << " }\n";
	}
	void field(std::string& value, cstring name){
		std::cout << " Field { name = " << name
				  << " ; type = std::string "
				  << " ; value = " << value << " }\n";	
	}

};


/** Serialize class data to stream in text format. */
struct WriterVisitor{
	using cstring = const char*;
	std::ostream& _os;

	WriterVisitor(std::ostream& os): _os(os) { }

	template<class Described>
	void visit(Described& desc){
		desc.describe(*this);
	}
	void name(const std::string& className){		
	}
	template<class T>
 	void field(T& x, cstring name){
		_os << x << " ";
	}
};

struct ReadVisitor{
	using cstring = const char*;
	std::istream& _is;

	ReadVisitor(std::istream& is): _is(is) { }

	template<class Described>
	void visit(Described& desc){
		desc.describe(*this);
	}
	void name(const std::string& className){		
	}
	template<class T>
 	void field(T& value, cstring name){
		_is >> value;
	}
};


/** Serialize class data to stream in binary format. */
struct SerializeVisitor{
	using cstring = const char*;
	std::ostream& _os;

	SerializeVisitor(std::ostream& os): _os(os) { }

	template<class Described>
	void visit(Described& desc){
		desc.describe(*this);
	}
	void name(const std::string& className){		
	}
	template<class T>
  	void field(T& value, cstring name){
		_os.write((char*) &value, sizeof(T));
	}	
	void field(std::string& x, cstring name){
		//std::cerr << " [LOG] String serializer" << "\n";
		auto n = x.size();
		_os.write((char*) &n , sizeof(size_t));
		_os.write((char*) x.data(), n * sizeof(char));
	}
};


struct DeserializeVisitor{
	using cstring = const char*;
	std::istream& _is;

	DeserializeVisitor(std::istream& is): _is(is) { }

	template<class Described>
	void visit(Described& desc){
		desc.describe(*this);
	}
	void name(const std::string& className){ }
	
	template<class T>
  	void field(T& value, cstring name){
		_is.read((char*) &value, sizeof(T));
	}
	
	void field(std::string& x, cstring name){
		size_t n = 0;
		_is.read((char*) &n, sizeof(size_t));
		std::cerr << " [LOG] N = " << n << std::endl;
		x.resize(n);
		_is.read((char*) &x[0], n * sizeof(char));
	}

};

//------------ Test class ------------------//

struct AClass{
	std::string name;
	int    n;
	double k;
	long   x;

	AClass(std::string name, int n, double k, long x)
	  : name(name), n(n), k(k), x(x)  { }

    // Delegated constructor
	AClass(): AClass("unnamed", 10, 25.0, 1000) { }

	// Note: This method should be in the header file.
	// Every serializable class implements
	// a templated member function "describe"
	// which describe the class structure to
	// an visitor object.
	template<class Visitor>
	void describe(Visitor& v){
		v.name("AClass");   
		v.field(n, "n");
		v.field(k, "k");
		v.field(x, "x");
		v.field(name, "name");		
	}
};

/** Turn non-printable characters into hexadecimal notation, for instance,
 *  the character '\n', 0x0A is printed as \x0A and 'a' is printed as 'a'.
 *  Requires: <string>, <sstream> and <iomanip>
 *------------------------------------------------------------------------*/
auto stringToHex(const std::string& str) -> std::string {
	auto os = std::stringstream{};
	for(const auto& ch: str)
		if(std::isprint(ch))
			os << ch << " ";
		else
			os << "\\x" << std::hex << std::setw(2) << std::setfill('0')
			   << (0xFF & ch) << ""
			   << std::dec;
	return os.str();
}

int main(int argc, char **argv)
{
	DescriptionVisitor descVisitor;
	AClass cls1("objectA", 200, -2.34, 900);

	std::cout << "\n===== EXPERIMENT 1 ===========" << std::endl;
	descVisitor.visit(cls1);

	std::cout << "\n===== EXPERIMENT 2 ===========" << std::endl;
	cls1.describe(descVisitor);

	std::cout << "\n===== EXPERIMENT 3 - Text Serialization/Deserialization ===" << std::endl;
	auto pseudoFile = std::stringstream{};
	auto writer     = WriterVisitor(pseudoFile);
    writer.visit(cls1);
	std::cout << " ==> pseudoFile = " << pseudoFile.str() << "\n" << std::endl;
	
	auto reader = ReadVisitor(pseudoFile);
	AClass clsb;
	reader.visit(clsb);
	descVisitor.visit(clsb);

	std::cout << "\n===== EXPERIMENT 5 == Serialize to file ===========" << std::endl;
	auto mockFile = std::stringstream{};
	auto serializer = SerializeVisitor(mockFile);
	serializer.visit(cls1);
	
	std::cout << "Stream = " << stringToHex(mockFile.str()) << std::endl;
	
	std::cout << "\n===== EXPERIMENT 6 == Deserialize from file ===========" << std::endl;
	auto deserializer = DeserializeVisitor(mockFile);
	AClass cls2;
	deserializer.visit(cls2);
	descVisitor.visit(cls2);

	return 0;
}

