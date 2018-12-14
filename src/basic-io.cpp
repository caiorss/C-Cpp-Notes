#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

int main(){	
	// Everything is initialized as zero
	double price, sum, min, max;
	std::vector<double> dataset;
	sum = 0.0;
	min = 1e10;
	max = -1e10;		

	// Print float point numbers with 2 decimal places and fixed notation 
	std::cout << std::setprecision(2) << std::fixed;
	std::cout << "Enter the prices and type CTRL+D you are done." << "\n";
	
	// Execute this code block while there is any error on the stream.
	// It means, the user did not type any bad input or
	//  CTRL + D (Send EOF End of File - signal)
	int n = 0;
	while(std::cin.good()){
		// (i++) Post increment i, increment variable i after it is print.
		std::cout << " => price[" << n++ << "] = ";
		// Read the price from stdin (standard input or console). 
		std::cin >> price;
		if(!std::cin.good()){
			std::cerr << " [LOG] " << "User typed invalid input. Exit loop." << "\n";
			// Force exiting the loop 
			break;
		}
		sum += price;
		// Save 
		dataset.push_back(price);
		if(price > max)
			max = price;
		if(price < min)
			min = price;
	}
	// Clear stream error flag 
	std::cin.clear();

	std::cout << "Sum  = " << sum       << "\n";
	std::cout << "Mean = " << (sum / n) << "\n";
	std::cout << "Min  = " << min       << "\n";
	std::cout << "Max  = " << max       << "\n";

	std::cout << "Dataset = [ ";
	for(size_t i = 0; i < dataset.size(); i++)
		std::cout << " " << dataset[i] ;
	std::cout << "] \n";

    #ifdef _WIN32 
       std::cout << "\n" << "Type RETURN to quit." << "\n";
       std::cin.clear(); 
       std::cin.ignore(10, '\n');
       // Wait for single character 
       std::cin.get();
     #endif 
		
	return EXIT_SUCCESS;
};
