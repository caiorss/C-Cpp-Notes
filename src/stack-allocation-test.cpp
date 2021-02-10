// File:   stack-allocation-test 
// Brief:  Test limits of stack or heap allocation. 
// Author: Caio Rodrigues
//------------------------------------------------------
#include <iostream>
#include <string> 

#ifndef ARRAY_SIZE
   #define ARRAY_SIZE 8000
#endif 

int main(){
	size_t sizeKB = static_cast<size_t>(ARRAY_SIZE) * sizeof(double) / 1024;
	std::cout << " [LOG] Number of elements = " << ARRAY_SIZE << "\n";
	std::cout << " [LOG] Array size in Kbytes = " << sizeKB << "\n";
    #if !defined HEAP || HEAP == 0
  	  // ===> Stack allocation 
	  std::cout << " [LOG] Stack allocation." << "\n";
	  double array [ARRAY_SIZE];
	#else
	  // ===> Heap allocation 
	  std::cout << " [LOG] Heap (dynamic memory) allocation." << "\n";
	  // Note: It throws std::bad_alloc exception. 
	  double* array = new double [ARRAY_SIZE];	  	  
	  delete [] array;
	#endif
	  std::cout << " [LOG] Program ended gracefully OK." << "\n";
	return 0;
}
