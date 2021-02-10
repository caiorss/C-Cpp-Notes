
#include <iostream>
#include <string>
using namespace std;

int main()
{
  string s;
  
  s = "hello world c++ string" ;
  cout << "s = " << s << endl;

  s += " Append this to string" ;
  cout << "s = " << s << endl;

  return 0;
}
