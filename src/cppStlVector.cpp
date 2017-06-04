#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

/// Pass the vectors by value. It copies the vectors, so passing by
//  reference does not copies the vectors and it is more efficient.
//
vector<double> sumVectors1(vector<double> xs, vector<double> ys){
  vector<double> zs(xs.size());
  
  for (int i = 0; i < xs.size(); i++){
    zs.at(i) = xs.at(i) + ys.at(i);
  }

  return zs;
}

vector<double> sumVectors2(vector<double> &xs, vector<double> &ys){
  vector<double> zs(xs.size());
  
  for (int i = 0; i < xs.size(); i++){
    zs.at(i) = xs.at(i) + ys.at(i);
  }

  return zs;
}


void printVector1(string name, vector<double> xs){
  cout << name << " = [ "; 
  
  for (int i = 0; i < xs.size(); i++){
    cout << xs.at(i) << " " ;
  }

  cout << "]" << endl;
}

void printVector2(string name, vector<double> &xs){
  cout << name << " = [ "; 
  
  for (int i = 0; i < xs.size(); i++){
    cout << xs.at(i) << " " ;
  }

  cout << "]" << endl;
}

void printVectorWithIterator(string name, vector<double> &xs){
  cout << name << " = [ "; 
  
  for (vector<double>::iterator i = xs.begin(); i != xs.end(); ++i){
    cout << *i << " " ;    
  }

  cout << "]" << endl;
  
}


// Returns the sum of all vector elements.
double vectorSum(vector<double> &xs){
  double sum = 0;
  
  for (int i = 0; i < xs.size(); i++){
    sum = sum + xs.at(i);
  }

  return sum;
}

// Returns the product of all vectors elements.
double vectorProd(vector<double> &xs){
  double prod = 1.0;
  
  for (int i = 0; i < xs.size(); i++){
    prod = prod + xs.at(i);
  }

  return prod;
}

double vectorNorm(vector<double> &xs){
  double sum = 1.0;
  double x   = 0.0;
  
  for (int i = 0; i < xs.size(); i++){
    x = xs.at(i);
    sum = sum + x * x;
  }

  return sqrt(sum);
}


double vectorAverage(vector<double> &xs){
  double sum = 1.0;
  
  for (int i = 0; i < xs.size(); i++){    
    sum = sum + xs.at(i);
  }

  return sum / xs.size();
}


void vectorAnalytics(string name, vector<double> &xs){
  cout << "\n\nAnalytic for vector " << name     << endl;
  cout << "----------------------------------- " << endl;
  printVector2(name, xs);
  cout << "size    = " << xs.size()         << endl;
  cout << "average = " << vectorAverage(xs) << endl;
  cout << "sum     = " << vectorSum(xs)     << endl;
  cout << "product = " << vectorProd(xs)    << endl;
  cout << "norm    = " << vectorNorm(xs)    << endl;
  
}


int main(){
  int size = 6;

  //  Vector initialized from a double array.
  vector<double> xs({1, 2, 3, 5.5, 10.8, 4.5});

  // Vector initialized with all elements set to 0.0
  vector<double> ys(6);

  // Vector intialized with all elements set to 4.0
  vector<double> zs(6, 4.0);

  // Vector with 0 elements.
  vector<double> ks;

  printVector1("xs", xs);

  printVector1("ys", ys);

  printVector2("zs", zs);

  printVectorWithIterator("Print Vector with iteractor - xs", xs);
  
  cout << "xs.size()      = " << xs.size()  << endl ;
  cout << "xs.at(3)       = " << xs.at(3)   << endl ;    
  cout << "xs.at(0)       = " << xs.at(0)   << endl ;
  cout << "xs.at(5)       = " << xs.at(5)   << endl ;      
  cout << "vectorSum(xs)  = " << vectorSum(xs) << endl;
  cout << "vectorNorm(xs) = " << vectorNorm(xs) << endl;
  
  printVector1("sumVectors1(xs, zs) = xs + zs", sumVectors1(xs, zs));
  printVector1("sumVectors2(xs, zs) = xs + zs", sumVectors2(xs, zs));

  cout << "Clear vector xs -> "  ;
  xs.clear();
  printVector2("xs", xs);

  ks.push_back(4.0);
  ks.push_back(5.0);
  ks.push_back(3.0);
  ks.push_back(6.0);
  ks.push_back(7.0);
  ks.push_back(9.3);
  printVector2("ks", ks);
  
  vectorAnalytics("ks", ks);
  
  return 0;
}
