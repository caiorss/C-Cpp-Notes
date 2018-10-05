
#include <iostream>  // Basic IO functions 
#include <cmath>     // C-math functions such as sqrt, pow ... 
#include <cassert>   // Assertions 

using namespace std;

// Function prototypes
//---------------------------------//

double scalarProduct   (double [], double [], int);
double vectorSum       (double [], int);
double vectorNorm      (double [], int);
void   printVectorVert (double xs[], int size);

//  Main function 
//-------------------------------
  
int main()
{
  int size = 4 ;
  double v1[] = {1.0, 2.0, 3.0, 4.0} ;
  double v2[] = {3.0, 4.0, 5.0, 6.0} ; 
  
  cout << "\n\nVector v1 = " << endl ;
  printVectorVert(v1, size);
  
  cout << "\n\nVector v2 = " << endl ; 
  printVectorVert(v2, size);
  
  cout << "\n\nScalar product is = " << scalarProduct(v1, v2, size) << endl;
  
  cout << "Vector v1 sum is  = " << vectorSum(v1, size) << endl;

  cout << "Vector v2 Norm is = " << vectorNorm(v1, size) << endl;
  
  assert(scalarProduct(v1, v2, size) == 50.0);
  
  return 0 ; // Return 0 as status code.
}

//  Functions Implementations
// -------------------------------- //

double scalarProduct(double xs[], double ys[], int size){
  double sum = 0.0;

    for (int i = 0; i <= size -1; i++){
      sum = sum + xs[i] * ys[i];
    }
  return sum;
}

double vectorSum(double xs[], int size){
  double sum = 0.0 ;

  for (int i = 0; i <= size - 1; i++){
    sum = sum + xs[i];
  }
  return sum;
}


double vectorNorm(double xs[], int size){
  double sum = 0.0 ;

  for (int i = 0; i <= size - 1; i++){
    sum = sum + xs[i] * xs[i];
  }
  return sqrt(sum);
}


void printVectorVert(double xs[], int size){
  for (int i = 0; i <= size -1; i++){
    cout << "v[" << i << "] = " << xs[i] << endl ;
  }
}
