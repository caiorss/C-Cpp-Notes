// File:   polymorphic-io1.cpp 
// Brief:  Demonstration about how to write polymorphic IO code or I/O agnostic code.
// Author: Caio Rodrigues
//-------------------------------------------------------------------------------------

#include <iostream>
#include <ostream> // std::ostream -> Root Output class
#include <istream> // std::istream -> Root Input class
#include <fstream> // File.
#include <sstream> // string stream
#include <vector>
#include <iomanip>
#include <string>

namespace VectorIO{
	/** Write vector of doubles to any output stream. */
	auto writeVector(std::ostream& os, const std::vector<double>& xs ) -> void;
	// Version with R-value reference can accept temporary objects. 
	auto writeVector(std::ostream&& os, const std::vector<double>& xs) -> void;

	/** Read vector of doubles from any input stream. */
	auto readVector(std::istream& is) -> std::vector<double>;
	auto readVector(std::istream&& is) -> std::vector<double>;
}

int main(){
	using VectorIO::writeVector;
	using VectorIO::readVector;
	
	std::vector<double> vtest = {2.0, 4.0, 5.0, 10.0, 3.45, 9.5};

	std::cout << "\n TEST1 Writing to stdout (Standard output stream) " << "\n";
	writeVector(std::cout, vtest);

	std::cout << "\n TEST2 Writing to stderr (Standard error stream) " << "\n";
	writeVector(std::cerr, vtest);

	std::cout << "\n TEST3 Writing to std::stringstream " << "\n";
	std::stringstream fakeFile;
	writeVector(fakeFile, vtest);	
	std::cout << "fakeFile = " << fakeFile.str() << "\n";

	std::cout << "\n TEST4 Reading from std::stringstream " << "\n";
	auto out1 = readVector(fakeFile);
	writeVector(std::cout, out1);

	std::cout << "\n TEST5 Write to a file" << "\n";
	// Note: It is not possible without R-value reference.
	writeVector(std::ofstream("vector.txt"), vtest);
	
	std::cout << "\n TEST6 Read from file - Version 1" << "\n";
	// Call R-value reference version 
	auto out2 = readVector(std::ifstream("vector.txt"));
	writeVector(std::cout, out2);

	std::cout << "\n TEST7 Read from file - Version 2" << "\n";
	std::ifstream fd{"vector.txt"};
	// Call L-value reference version of readVector 
	auto out3 = readVector(fd);
	writeVector(std::cout, out3);	
}

namespace VectorIO{
	auto writeVector(std::ostream& os, const std::vector<double>& xs) -> void
	{
		os << "VECTOR";
		os << " ";
		for(auto x: xs)
			os << x << " ";
		os << "\n";
		os.flush();
	}
	auto writeVector(std::ostream&& os, const std::vector<double>& xs) -> void
	{
		std::cerr << " [LOG] (writeVector) R-value reference" << "\n";
		writeVector(os, xs);
	}
	auto readVector(std::istream& is) -> std::vector<double>
	{
		std::vector<double> xlist;
		std::string label;
		is >> label;
		if(label != "VECTOR")
			throw std::runtime_error("Error: wrong file layout.");
		double x;
		while(is.good() && !is.eof()){
			is >> x;
			xlist.push_back(x);
		}
		return xlist;
	}
	auto readVector(std::istream&& is) -> std::vector<double>
	{
		std::cerr << " [LOG] (readVector) R-value reference" << "\n";
		return readVector(is);
	}
}
