// Brief:  C++17 Structured bindings experiments.
// Author: Caio Rodrigues 
//--------------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <tuple>
#include <iomanip>
#include <algorithm>

int main(){	
	
	{
		std::puts("\n=== EXPERIMENT 1: Decompose: pair - binding - pair =====");
		auto p = std::pair<std::string, int>("C++", 17);
		auto [name, version] = p;
		std::cout << " name = " << name << " ; version = " << version << std::endl;
	}

	{
		std::puts("\n=== EXPERIMENT 2: Decompose: map / 'hash-table' - =====");
		auto database = std::map<std::string, int>{{"c++", 17}, {"rust", 10}, {"Forth", 200}};
		for(const auto& [key, value] : database)
			std::cout << "key = " << key << " ; value = " << value << std::endl;		
	}

	{
		std::puts("\n=== EXPERIMENT 3: Decompose: tuple - =====");
		using DatabaseRow = std::tuple<int, std::string, double>;
		auto database = std::vector<DatabaseRow>{
			{100, "Fried tasty fresh cheese",   3.45},
			{400, "Super hot toasted coffee.", 6.25},
			{500, "Fresh Orange Juice",        4.50},
		};

		for(const auto& [id, name, price]: database)
			std::cout << " ROW=> id = " << id
					  << " ; name = "   << name
					  << " ; price = "  << price 
					  << std::endl;
	}

	{
		std::puts("\n=== Decompose: tuple with Reference - =====");
		using DatabaseRow = std::tuple<int, std::string, double>;
		auto row = DatabaseRow{200, "Coffee", 4.5};
		auto& [id, name, price] = row;

		std::printf(" [BEFORE] => Product{ id = %d ; name = %s ; price = %f }\n",
					std::get<0>(row), std::get<1>(row).c_str(), std::get<2>(row));

		id = 500, name = "italian-sytle coffee", price = 10.60;

		std::printf(" [AFTER] => Product{ id = %d ; name = %s ; price = %f }\n",
					std::get<0>(row), std::get<1>(row).c_str(), std::get<2>(row));
	}

	{
		std::puts("\n=== EXPERIMENT 4: Decompose: Structs - =====");
		struct GeoCoordinate{
			std::string name;
			double      latitude;
			double      longitude;
		};
		auto geoDatabase = std::deque<GeoCoordinate>{
			 {"Bogota",         4.7110,  -74.0721}
			,{"Beijing",       39.9042,  116.4074}
			,{"Gauteng",      -26.2708,   28.1123}			
			,{"Buenos Aires", -34.6037,  -58.3816}
			,{"Brasilia",     -15.8267,  -47.9218}
		};
		std::cout << std::setprecision(3);
		std::for_each(geoDatabase.begin(), geoDatabase.end(),
					  [](const auto& city){
						  const auto& [name, lat, lon] = city;
						  std::cout << std::setw(15) << std::left  << name
									<< std::setw(8) << std::right << lat
									<< std::setw(8) << lon 
									<< "\n";						  
						  
					  }); 
	}

	{
		std::puts("\n=== EXPERIMENT 5: Decompose: C-Array - =====");
		double array [3] = {10.23, 90.23, 100.0};
		auto [x, y, z] = array;
		std::printf(" array { x = %.3f ; y = %.3f ; z = %.3f }", x, y, z);
		
	}		
	return 0;
}
