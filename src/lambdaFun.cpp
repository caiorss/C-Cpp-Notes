//  Description: Proof-of-concept code to show all C++11 lambda capabilities.
//  Objective :   Show all possible C++11 Lambda variations and use cases in a single-file code.
//
#include <iostream>
#include <functional>  // Provides lambda function types
#include <vector>
#include <algorithm> // Import for_each and transform
#include <string>
#include <fstream>

using namespace std;

// Example (1)
auto lambda1 = [](const std::string& str){
  cout << "Lambda1: : I got the value = " << str << endl;
};

void showVector(const char* name, vector<int> xs){
  int n = 0;
  for_each(xs.begin(), xs.end(),
           [&](int k) {
             cout << name << "  [" << n << "] = " << k << endl;
             ++n;
           });
}

/*  Example (7) -- Return a function that adds x to a number.
 *  Lambda functions can be returned from functions
 */
function<int (int, int)> makeFunction1(int m){
    return [m](int x, int y){ return m * (x + y); };
}

/*  Example(8) - Returning a stateful lambda function from a function.
 */
auto  makeFunction2(int m){
  int n = 0;
  return [m, &n](int x, int y){
    int z = m * (x + y) + n;
        cout << "  Running dummy function" << endl;
        cout << "    (*) m = " << m  << ", n = " << n << ", x = " << x << ", y = " << y  << ", z = " << z  <<  endl;
    n++;
  };
}

//  Example (11) - Pass a function as argument
//
int foldVector(vector<int> xs, int init, function<int (int, int)> fn){
        int acc = init;
        for (int i = 0; i < xs.size(); i++){
                acc = fn(acc, xs[i]);
        }
        return acc;
}

//  Example (12) - Observer pattern with observer objects 
//  replaced by lambda functions
// 
class Observable{
protected:
        vector<function<void (double)>> observers;

public:
        Observable(){}

        void addObserver(function<void (double)> obs){
                observers.push_back(obs);
        }

        void notify(double temp){
                for(auto itr = observers.begin(); itr != observers.end(); itr++){
                        (*itr)(temp);
                }
        }
};

int main(){

  //-----------  Example(1) ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(1) - Testing function lambda1" << endl;
  lambda1("Hello");
  lambda1("World");

  //-----------  Example(2) ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(2) Testing function lambda2" << endl;
  // Example (2)
  auto lambda2 = [](const char* str) {
    cout << "Lambda 2 :: I got the value  = " << str << endl;
  };

  lambda2("Japan");
  lambda2("Korea");

  //-----------  Example(3) ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(3) Testing function lambda3" << endl;
  cout << "   - Lambda function can be defined and executed at the call-site" << endl;

  [](const char* str) -> void {
    cout << "Lambda 3 :: I got the value = " << str << endl;
  }("C++11 is awesome!");

  //----------- Example (4)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(4) Testing function sumLambda" << endl;
  cout << "   - Lambda functions  can return values as any function." << endl;

  auto sumLambda = [](double x, double y){
    return x + y;
  };

  cout << "sumLambda(10.0, 25.34) = " << sumLambda(10.0, 25.34) << endl;
  cout << "sumLambda(-10.23, 4.56) = " << sumLambda(-10.23, 4.56) << endl;

  //---------- Example (5)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(5) Testing function lamdaCapture" << endl;
  cout << "   - Lambda functions  can capture its environment (closures) and have state as \"function objects\"" << endl;

  int x = 5;;
  int c = 3;
  // Pass x by copy(by value) and c by reference (c can be modfied)
  //
  auto lamdaCapture = [x, &c](int a, int b){
    int z = 2 * a + b * x + c;
    c = c + 1;
    // x = x + 2   ---> Will trigger a compile error.
    //  ( error: assignment of read-only variable ‘x')

    return z;
  };

  cout << "  x  = " << x << "  c = " << c << endl;
  cout << "lamdaCapture(2, 5)  = " << lamdaCapture(2, 5) << endl;

  cout << "  x  = " << x << "  c = " << c << endl;
  cout << "  lamdaCapture(1, 2) =  " << lamdaCapture(1, 2) << endl;

  cout << "  x  = " << x << "  c = " << c << endl;

  //---------- Example (6)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(6) Testing function sumLambda" << endl;
  cout << "   - Lambda functions  can play well with STL algorithms" << endl;

  vector<int> dataset{1, 2, 3, 4, 5, 6, 7, 8};

  cout << "Print all vector elements - Version 1" << endl;

  int index = 0;
  for_each(dataset.begin(), dataset.end(), [&index](int k){
      cout << "  v[" << index << "] = " << k << endl;
      index = index + 1;
    });

  cout << "Print all vector element Version 2  - local state with 'static' keyword"  << endl;
  for_each(dataset.begin(), dataset.end(), [&index](int k){
     // this variable n will keep its state on each call
      static int idx;
      cout << "  v[" << idx << "] = " << k << endl;
      idx++;
    });

  //---------- Example (7)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(7) " << endl;
  cout << "   Playing with STL transform algorithm." << endl;
  cout << "  Vector transformed =  " << endl;
  transform(
            dataset.begin(),
            dataset.end(),
            dataset.begin(),
            [](int x){ return x * 3 + 1; }
            );

  showVector("v", dataset);

  //---------- Example (8)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(8) " << endl;
  cout << "   Lambda functions can be returned from functions and be generated at run-time." << endl;

  auto mulSumBy2 = makeFunction1(2);
  cout << "  mulSumBy2(2, 4) = " << mulSumBy2(2, 4) << endl;
  cout << "  mulSumBy2(3, 1) = " << mulSumBy2(3, 1) << endl;

  auto mulSumBy5 = makeFunction1(5);
  cout << "  mulSumBy5(2, 4) = " << mulSumBy5(2, 4) << endl;
  cout << "  mulSumBy5(9, 2) = " << mulSumBy5(9, 2) << endl;

  //---------- Example (9)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(9) " << endl;
  cout << "   Lambda functions can be returned from functions and have state." << endl;

  auto dummyFun1 = makeFunction2(2);
  dummyFun1(1, 3);
  dummyFun1(2, 5);

  auto dummyFun2 = makeFunction2(3);
  dummyFun2(1, 3);
  dummyFun2(2, 5);

  //---------- Example (10)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(10) " << endl;
  cout << "   Lambda functions can return lambda functions!!" << endl;

  auto makeAdder = [](int x){
          // cout << "x = " << x << endl;
          return [x](int y){ return x + y ; };
   };
  auto addTo5  = makeAdder(5);
  cout << "  addTo5(4) = " << addTo5(4) << endl;
  cout << "  addTo5(3) = " << addTo5(3) << endl;

  auto addTo10 = makeAdder(10);
  cout << "  addTo10(4) = " << addTo10(4) << endl;
  cout << "  addTo10(3) = " << addTo10(3) << endl;

  //---------- Example (11)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(11) " << endl;
  cout << "   Lambda functions can be passed as function arguments!" << endl;

  vector<int> dataset2 = {1, 2, 3, 4, 5, 6, 7};
  auto add = [](int x, int y){ return x + y; };
  auto mul = [](int x, int y){ return x * y; };
  cout << "foldVector(dataset2, 0, add) = " << foldVector(dataset2, 0, add) << endl;
  cout << "foldVector(dataset2, 1, mul) = " << foldVector(dataset2, 1, mul) << endl;  

  //---------- Example (12)  ----------------------------------------------------------------------------------------
  cout << endl;
  cout <<"-----------------------------------------------------------------------------------------" << endl;
  cout << ">> Example(12) - Observer pattern" << endl;

  Observable tempSensor = Observable(); 
  tempSensor.addObserver([](double temp){
                  cout << "(observer 1) Temperature changed to " << temp <<  " C" << endl;
          });
  auto observer2 = [](double temp){
          cout << "(observer 2) Sensor temperature changed to " << temp <<" C" << endl;
  };
  tempSensor.addObserver(observer2);

  tempSensor.notify(30.5);
  tempSensor.notify(20.5);  

  return 0;
}
