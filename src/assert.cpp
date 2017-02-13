
// Description: Assertion demonstration in C++.
// Author:      Caio Rodrigues
// File:        assert.cpp
// Compile: g++ assert.cpp -o assert.bin
//

#include <cassert>    // Assertion
#include <iostream>   // Standard library

int main (int argc, char * argv[]) {

  // Check argc and exit if not enough arguments are provided to avoid segmentation fault.
  //
  if (argc != 2){
    std::cout << "Usage: ./" << argv[0] << "[n]" << std::endl;
    std::cout << "where [n] is a number > 0. If n * n != 25 the assertion fails." << std::endl;

    exit(0); // exit(1) - Return status code 1 - Indicating failure.
  }

  int i = atoi(argv[1]);
  int j = i * i ;

  assert (j == 25);

  std::cout << "continuing" << std::endl;

  exit(0); // exit(0) - Return status code 0 - Indicating success.
}
