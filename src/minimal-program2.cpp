#include <iostream>
#include <string>

int main(int argc, char **argv, char **environ){
	if(argc < 2){
		std::cerr << "Error: invalid command line argument. Expected -args or -env "
				  << "\n";
		// Exit  immediately (return 1)
		return EXIT_FAILURE; 
	}

	if(std::string(argv[1]) == "-args"){
		std::cout << "Number of command line arguments = " << argc << "\n";
		std::cout << "argv[0] = " << argv[0] << "\n";
		std::cout << "\n";
		std::cout << "Showing command line arguments:" << "\n";
		std::cout << "+-----------------------------+" << "\n";
		for(int i = 0; i < argc; i++)
			std::cout << "argv[" << i << "] = " << argv[i] << "\n";
		return EXIT_FAILURE; 
	}
	
	if(std::string(argv[1]) == "-env"){
		std::cout << "Show environment variables" << "\n";
		char** env = environ;
		while(*(env++) != nullptr)
			std::cout << " => env = " << *env << "\n";
		return EXIT_FAILURE; 		
	}
	std::cerr << "Error: invalid argument <" <<  argv[1] << "> " <<"\n";     
	// return 0
	return EXIT_SUCCESS;
};
