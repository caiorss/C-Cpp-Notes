
// Description: atoi, atof, strtol demonstration.
// Note: Convert string to integer, double and long integer.
// Tags: atoi, atof, strtol
//

#include <cassert>
#include <iostream>

using namespace std;

int main (){

  cout << "\nAtoi - parse int 32 bits" << endl;

  cout << "atoi(\"2342\")    = " << atoi("2342")  << endl;
  cout << "atoi(\"-2323\")   = " << atoi("-2323") << endl;
  cout << "atoi(\"failed\")  = " << atoi("failed") << endl;
  cout << "atoi(\"12xy346\") = " << atoi("12xy346") << endl;

  cout << "\nAtof - parse float/double" << endl;

  cout <<  "atof(\"100.23\")              = "  << atof("100.23") << endl;
  cout <<  "atof(\"-20.015e3\")           = "  << atof("-20.015e3") << endl;
  cout <<  "atof(\"100.134354blablah\")   = "  << atof("100.134354blablah") << endl;
  cout <<  "atof(\"failed0.3123garbage\") = "  << atof("failed0.3123garbage") << endl;

  return 0;
}
