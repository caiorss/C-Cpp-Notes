#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>

// #include "hpc.hpp"

using namespace std;

/// it can be put in a separated header file.
//
extern "C" { double vector2DNorm(double x, double y); }

extern "C" { double vectorNorm(double [], int) ; }

extern "C" { double arrayNorm(double [], int); }

extern "C" { void* createVector(double [], int); }

extern "C" { double* genArray(int size); }

extern "C" { void genArray2(double* arr, int size); }

extern "C" {double vectorSum(double xs [], size_t n); }

/// C-wrapper for the circle class 
extern "C" {
  void  *Circle_new    (double radius);
  void   Circle_delete (void *circle);
  double Circle_getArea(void *circle);
}



// Create a C++ STL vector from an C-array.
//
vector<double> arrayToVector(double arr[], int n){
  vector<double> xs;
  for (int i = 0; i < n; i++){
    xs.push_back(arr[i]);
  }   
  return  xs;
}

vector<double> arrayToVector2(double arr[], int n){
  // arr       -> Is a pointer to the first array cell 
  // arr + n   -> Is a pointer ot the last array cell.
  vector<double> v(arr, arr + n);
  return v;
}


// extern "C" { void* createVector(double [], int); }

void* createVector(double xs [], int n){
  vector<double> v(xs, xs + n);
  return static_cast<void*> (& v[0]);
}





double arrayNorm(double xs [], int n){
  double acc = 0;
  
  for (int i = 0; i < n; i++){
    acc = acc + xs[i] * xs[i];
  }

  return sqrt(acc);
}


double vector2DNorm(double x, double y){
    return sqrt(x * x + y * y);
}


double vectorNorm(vector<double> &xs){
  double sum = 0.0;
  
  for (size_t i = 0; i < xs.size(); i++){
    sum = sum + xs.at(i) * xs.at(i);
  }

  return sqrt(sum);
}


// C-wrapper to vectorNorm
//
// extern "C" { double vectorNorm(double [], int) ; }
//
double vectorNorm(double xs [], int n){
  //vector<double> v = arrayToVector(xs, n);
  vector<double> v = arrayToVector2(xs, n);
  return vectorNorm(v);
}

double vectorSum(vector<double> &xs){
  double sum = 0.0 ;
  for (size_t i = 0; i < xs.size(); i++){
    sum = sum + xs.at(i);
  }
}

// C-wrapper for vectorSum 
// 
double vectorSum(double xs [], size_t n){
  vector<double>v(n);
  double* p = v.data();
  p = xs;
  return vectorSum(v);  
}



vector<double> vectorScale(vector<double> &xs, double scale){
  vector<double> ys;

  for (int i = 0; i < xs.size(); i++){
    ys.push_back(scale * xs.at(i));
  }
  return ys;
}


// void vectorScale(double xs [], int n, double* out){
//   vector<double> v = vectorScale(arrayToVector2(xs, n));
//   return v
// }

// Generates an array of size n
//
double* genArray(int size){
  double *arr = new double[size];

  for(int i = 0; i < size; i++){
    arr[i] = i * i;
  }

  return arr;
}

// Generates an array of size n
//
// Unlike genArray, in this function, the caller
// must allocate the array that will be returned.
//
void genArray2(double *xs, int size){
  for(int i = 0; i < size; i++){
    xs[i] = i * i;
  }
}



class Circle{
private:
  double radius;
  double PI = 3.1415;

public:
  Circle(){
    radius = 1.0;
  }

  Circle(double rad){
    radius = rad;
  }

  double getArea(){
    return radius * radius * PI;
  }

};


// Return a void pointer to the circle object.
//
void* Circle_new(double radius){
  return new Circle(radius);
}

void Circle_delete(void* circle){
  // Cast void pointer to circle
  Circle *c = (Circle *) circle;
  // Deallocate the memory.
  delete c;  
}

double Circle_getArea(void* circle){
  Circle *c = (Circle *) circle;
  return c->getArea();
}





