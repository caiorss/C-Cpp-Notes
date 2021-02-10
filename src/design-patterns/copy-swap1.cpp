// File:   copy-swap1.cpp
// Brief:  Demonstration of copy-and-swap idiom for implementing rule-of-three and rule-of-five.
// Author: Caio Rodrigues
//-----------------------------------------------------------------
#include <iostream>
#include <ostream>    // Operator (<<) overload for class ostream 
#include <utility>    // std::swap
#include <iterator>   // std::begin, std::end 
#include <algorithm> 
#include <cstring>    // strlen, strcpy, ... 
#include <cassert>    // assert() function 

//========== file: String.hpp - Header file ===========//

class String
{
public:
	// Default constructor 
	String();
	String(size_t size, char ch);
	String(const char* text);
	// Copy-constructor (rule-of-three and rule-of-five)
	String(const String& rhs);
	// Move-constructor (rule-of-three and rule-of-five)
	String(String&& rhs);
	// Destructor (rule-of-three and rule-of-five)
	~String();

	// Copy-assignment operator (rule-of-three and rule-of-five)
	String& operator= (const String& rhs);
	// Move-assignment operator (rule-of-five)
	String& operator= (String&& rhs);

	String operator+(const char* text);
	
	// Swap function (std::swap overload) 
	// needed by the copy-and-swap idiom.
	friend void swap(String& lhs, String& rhs);

	// Make class printable - note this operator is a member
	// function of class ostream, no from this class.
	friend std::ostream& operator<<(std::ostream& os, const String& rhs);

	size_t      size();
	const char* data();
private:	
	// String size 
	size_t m_size;
	// Resource: Pointer to first string character 
	char*  m_data;	
};

//========== file: String.hpp Implementation ===========//

String::String(size_t size, char ch)
{
	m_size = size;
	m_data = new (std::nothrow) char [size];
	std::fill(m_data, m_data + size, ch);
}

// Note: strlen returns the string size as the number of characters
// ,however it does not include the last sentinel character '\0'
// null character. 
String::String(const char* text)
	: String(strlen(text) + 1, 0x00)
{
	std::copy(text, text + strlen(text) + 1, m_data);
}

// Default/Null constructor
// Calls: String::String(const char* text)
String::String()
	: String(1, 0x00)
{	
}

// Destructor implementation  
String::~String()
{
	// Check for null to avoid dangling pointer.
	if(m_data != nullptr)
		delete [] m_data;
	m_data = nullptr;
	m_size = 0;	
}

// std::swap overload - Implementation of
// => friend void swap(String& lhs, String& rhs)
void swap(String& lhs, String& rhs){
	std::swap(lhs.m_data, rhs.m_data);
	std::swap(lhs.m_size, rhs.m_size);
}

// Copy-constructor
String::String(const String& rhs){
	this->m_size = rhs.m_size;
	this->m_data = new (std::nothrow) char [rhs.m_size + 1];
	std::copy(rhs.m_data, rhs.m_data + rhs.m_size + 1, this->m_data);
}

// Move-constructor
String::String(String&& rhs)
	: String() // Delegated constructor 
{
	swap(*this, rhs);
}

// Copy-assignment operator (rule-of-three and rule-of-five)
// Copy-and-swap implementation 
String& String::operator= (const String& rhs)
{
	// Temporary copy 
	String temp(rhs);
	swap(*this, temp);
	return *this;
}

// Move-assignment operator (rule-of-five)
// Transfer resource ownership from right hand-side object
// to this object being instantiated. 
String& String::operator= (String&& rhs){
	swap(*this, rhs);	
	return *this;
}

String String::operator+(const char* text)
{
	String str(m_size + strlen(text) + 1, 0x00);
	std::copy(m_data, m_data + m_size, str.m_data);
	std::copy(text, text + strlen(text), str.m_data + m_size - 1);
	return str;
}

size_t String::size()
{
	return m_size;
}

const char* String::data()
{
	return m_data;
}


// Implementation of:
// => friend std::ostream& operator<<(std::ostream&, const String& rhs)
std::ostream& operator<<(std::ostream& os, const String& rhs)
{
	return os << rhs.m_data;
}


// ----------------------------------------//

int main()
{
	std::puts("\n =========== Test 1 ==============");
	// Empty 
	String s1;
	std::cout << "s1 = '" << s1 << "' ; size = " << s1.size() << std::endl;

	std::puts("\n =========== Test 2 ==============");
	String s2("hello world C++ copy-and-swap - São Paulo, São João da Barra");
	std::cout << "s2 = '" << s2 << "' ; size = " << s2.size() << std::endl;

	// Invoke copy-constructor 
	String s2copy("C++ copy-and-swap1");
	std::cout << " [BEFORE] s2Copy = '" << s2copy << "' ; size = " << s2copy.size() << std::endl;

    // Invoke copy-assignment operator 
	s2copy = s2;
	std::cout << " [AFTER1]  s2Copy = '" << s2copy << "' ; size = " << s2copy.size() << std::endl;

	std::puts("\n =========== Test 3 ==============");
	// Invoke move-assignment operator
	s2copy = String("move-assignment operator");
	std::cout << " [AFTER2]  s2Copy = '" << s2copy << "' ; size = " << s2copy.size() << std::endl;

	// Invoke move-constructor
	// Note: It will prevent copy-elison 
	String s3 = std::move(String("move-constructor") + " - hello world");
	std::cout << " s3 = '" << s3 << "' ; size = " << s3.size() << std::endl;

	std::puts("\n =========== Test 4 ==============");
	String s4 = "string s4";
	std::swap(s3, s4);
	std::cout << " s4 = '" << s4 << "' - s3 = '" << s3 << "'" << std::endl;

	std::puts("\n =========== Test 5 ==============");	
	// Invoke move-assignment operator 
	s3 = std::move(s2);
	std::cout << " [MOVE] s3 = " << s3 << std::endl;
	
	return 0;
}
