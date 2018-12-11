//---------------------------------------------------------------
// File:        GetLogicalDrivers.cpp 
// Description: Enumerate all logical drivers in the computer. 
// Author:      Caio Rodrigues
//--------------------------------------------------------------

#include <iostream>
#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <functional>
#include <Windows.h>

using DriverEnumerator = std::function<auto (std::string) -> void>;
auto EnumerateLogicalDriver(DriverEnumerator Consumer) -> void;

template<template<class, class> class Container = std::vector>
auto LogicalDriverList()
	-> Container<std::string, std::allocator<std::string>>;

int main(){
	std::puts("Logical Drivers or Disks found in the current installation");
	std::puts("------------------------------------------------");
	
	EnumerateLogicalDriver(
		[](const std::string& name){
			std::cout << "Driver = " << name << std::endl;
		});

	auto driverList1 = LogicalDriverList<std::deque>();
	std::cout << " *=> Driver list in STL container std::deque  = ";
	for(const auto& d: driverList1){ std::cout << d << ", "; }
	std::cout << std::endl;;
	
	auto driverList2 = LogicalDriverList<std::vector>();
	std::cout << " *=> Driver list in STL container std::vector = ";
	for(const auto& d: driverList2){ std::cout << d << ", "; }
	std::cout << std::endl;;

	return 0;
}

//---------------------------------------------------// 

using DriverEnumerator = std::function<auto (std::string) -> void>;

auto EnumerateLogicalDriver(DriverEnumerator Consumer) -> void 
{
	size_t size = ::GetLogicalDriveStringsA(0, nullptr);
	std::string buffer(size, 0x00);
	::GetLogicalDriveStringsA(buffer.size(), &buffer[0]);
	std::stringstream ss{buffer};
	while(std::getline(ss, buffer, '\0') && !buffer.empty())
		Consumer(buffer);
}

// Remember: Template always in header files 
// The user can choose the type of container used to return
// the computer drivers. 
template<template<class, class> class Container = std::vector>
auto LogicalDriverList()
	-> Container<std::string, std::allocator<std::string>>
{	
	Container<std::string, std::allocator<std::string>> list{};
	EnumerateLogicalDriver(
		[&](const std::string& name){
			list.push_back(name);
		});
	return list;	
}
