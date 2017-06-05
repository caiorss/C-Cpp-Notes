#include <iostream>
#include <cmath>
#include <vector>
// #include "hpc.hpp"

using namespace std;

/// it can be put in a separated header file.
//
extern "C" { double vector2DNorm(double x, double y); }

extern "C" { double vectorNorm(double [], int) ; }

extern "C" { double arrayNorm(double [], int); }


// Create a C++ STL vector from an C-array.
//
vector<double> arrayToVector(double arr[], int n){
  vector<double> xs;
  for (int i = 0; i < n; i++){
    xs.push_back(arr[i]);
  }   
  return  xs;
}



/// C-wrapper for the circle class 
extern "C" {
  void  *Circle_new    (double radius);
  void   Circle_delete (void *circle);
  double Circle_getArea(void *circle);
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
  
  for (int i = 0; i < xs.size(); i++){
    sum = sum + xs.at(i) * xs.at(i);
  }

  return sqrt(sum);
}

// C-wrapper to vectorNorm
//
// extern "C" { double vectorNorm(double [], int) ; }
//
double vectorNorm(double xs [], int n){
  vector<double> v = arrayToVector(xs, n);
  return vectorNorm(v);
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





