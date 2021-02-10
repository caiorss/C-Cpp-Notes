/**   File:      dataTypeSizes.cpp 
      Objective: Show C/C++ primitive data types sizes.      

 */

#include <iostream>
#include <limits>

using namespace std;


int main(){
  // 8 bits / 1 byte numeric types
  //---------------------------------------------------
  int charMin = numeric_limits<char>::min() ;
  int charMax = numeric_limits<char>::max() ;
  
  int scharMin = numeric_limits<signed char>::min() ;
  int scharMax = numeric_limits<signed char>::max() ;

  int ucharMin = numeric_limits<unsigned char>::min() ;
  int ucharMax = numeric_limits<unsigned char>::max() ;

  int int8Min  = numeric_limits<int8_t>::min() ;
  int int8Max  = numeric_limits<int8_t>::max() ;  

  int uint8Min  = numeric_limits<uint8_t>::min() ;
  int uint8Max  = numeric_limits<uint8_t>::max() ;

  // 16 bits / 2 bytes Numeric types
  //---------------------------------------------------
  
  int shortMin  = numeric_limits<short>::min() ;
  int shortMax  = numeric_limits<short>::max() ;

  int ushortMin  = numeric_limits<unsigned short>::min() ;
  int ushortMax  = numeric_limits<unsigned short>::max() ;
  
  int int16Min  = numeric_limits<int16_t>::min() ;
  int int16Max  = numeric_limits<int16_t>::max() ;  

  int uint16Min  = numeric_limits<uint16_t>::min() ;
  int uint16Max  = numeric_limits<uint16_t>::max() ;  

  // 32 bits / 4 bytes Numeric types
  //---------------------------------------------------
    
  int intMin  = numeric_limits<int>::min() ;
  int intMax  = numeric_limits<int>::max() ;  
  
  cout << "C/C++ Data type sizes in number of bytes (1 byte = 8 bits)" << endl;

  cout << "\n1 byte / 8 bits Data types"                       << endl;
  cout <<  "------------------------------------"              << endl; 
  cout << "sizeof(char)          = " << sizeof(char)           << " min = " << charMin  << " max = " << charMax << endl; 
  cout << "sizeof(signed char)   = " << sizeof(signed char)    << " min = " << scharMin << " max = " << scharMax << endl; 
  cout << "sizeof(unsigned char) = " << sizeof(unsigned char)  << " min = " << ucharMin << " max = " << ucharMax << endl; 
  cout << "sizeof(int8_t)        = " << sizeof(int8_t)         << " min = " << int8Min  << " max = " << int8Max << endl; 
  cout << "sizeof(uint8_t)       = " << sizeof(uint8_t)        << " min = " << uint8Min << " max = " << uint8Max << endl; 
  
  cout << "\n2 bytes / 16 bits Data types"                     << endl;  
  cout <<  "------------------------------------"              << endl;   
  cout << "sizeof(short)         = " << sizeof(short)          << " min = " << shortMin  << " max = " << shortMax  << endl; 
  cout << "sizeof(usigned short) = " << sizeof(unsigned short) << " min = " << ushortMin << " max = " << ushortMax << endl; 
  cout << "sizeof(int16_t)       = " << sizeof(int16_t)        << " min = " << int16Min  << " max = " << int16Max  << endl;  
  cout << "sizeof(uint16_t)      = " << sizeof(uint16_t)       << " min = " << uint16Min << " max = " << uint16Max << endl;  
  
  cout << "\n4 bytes / 32 bits Data types"                    << endl;  
  cout <<  "------------------------------------"             << endl;   
  cout << "sizeof(int)           = " << sizeof(int)           << " min = " << intMin << " max = " << intMax << endl;  
  cout << "sizeof(int32_t)       = " << sizeof(int32_t)       << endl;
  cout << "sizeof(int64_t)       = " << sizeof(int64_t)       << endl;
  
  cout << "sizeof(float)         = " << sizeof(float)         << endl;  
    cout << "\n8 bytes / 64 bits Data types"                    << endl;  
  cout <<  "------------------------------------"             << endl;    
  cout << "sizeof(long)          = " << sizeof(long)          << endl;
  cout << "sizeof(long long)     = " << sizeof(long long)     << endl;    
  cout << "sizeof(double)        = " << sizeof(double)        << endl;  
  
  return 0;
}
