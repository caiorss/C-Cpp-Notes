
#include <iostream>
using namespace std;

class Date
{
public:
  int year, month, day;

  // ---- Public Class Members ----- //
  
  void showDate();
  void showDate2();
  int  getYear();
  int  getDay();
  int  getMonth();
};


void Date::showDate(){
  cout << "Date = " << this->year << "-" << this->month << "-" << this->day << endl;
}

void Date::showDate2(){
  cout << "Date = " << year << "-" << month << "-" << day << endl;
}

int Date::getYear(){
  return year; 
}

int Date::getMonth(){
  return month;
}

int Date::getDay(){
  return day;
}

Date makeDate (int y, int m, int d){
  Date date;
  date.year  = y ;
  date.month = m ;
  date.day   = d ;
  return date;
}

void printDate(Date d){
  cout << "Date is " << d.year << "-" << d.month << "-" << d.day << endl;
}


int main(){
  Date d;
  d.day   = 10;
  d.month = 4;
  d.year  = 1998;

  cout << "Date (YMD) is = " << d.year << "-" << d.month << "-" << d.day << endl;  
  d.showDate();
  d.showDate2();  
  printDate(d);

  cout << "Year of date d is  = " << d.getYear() << endl;
  cout << "Month of date d is = " << d.getMonth() << endl;
  
  printDate(makeDate(1996, 8, 20));
   
  return 0;
}
