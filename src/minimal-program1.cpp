#include <iostream>
#include <string>

int main(){
	std::cout << "Hello world user" << "\n";
	std::cout << "Enter your name: ";
	std::string name;
	std::getline(std::cin, name);
	std::cout << "Be welcome to C++ land: " << name << "\n";

	// Wait user type a single character
   	std::cout << "\n" << "Type RETURN to exit";
    // Read a character from standard input (console)
    std::cin.get();
	
	// Status code 
	return 0;
}
