#include <iostream>
#include <string>
#include <iterator>
#include <iomanip>

// C++17 - Requires compiler linking flag: -lstdc++fs on CLang or GCC.
#include <filesystem>

namespace fs = std::filesystem;

/** Iterate over first N entries of a file system iterator. */
template<typename Range, typename Function>
auto dotimes(size_t n, Range&& iterable, Function fun){
	size_t i = 0;
	auto it = fs::begin(iterable);
	auto end = fs::end(iterable);
	while(i < n && it != end ){
		fun(it);
		++it;
		i++;
	}
}

int main(){
	
	std::cout << std::boolalpha;

	std::cout << "\n EXPERIMENT 1 ===== Checking files in the system." << std::endl;
	fs::path p1 = "/etc/iscsi/initiatorname.iscsi";
	std::cout << "          p1 = " << p1 << std::endl;
	std::cout << "p1.string()  = " << p1.string() << std::endl;
	std::cout << "p1 ? exists  = " << fs::exists(p1) << std::endl;
	std::cout << "p1 ? is File = " << fs::is_regular_file(p1) << std::endl;
	std::cout << "p1 ? is Dir  = " << fs::is_directory(p1) << std::endl;

	fs::path p2 = "/boot";
	std::cout << "          p2 = " << p2 << std::endl;
	std::cout << "p2.string()  = " << p2.string() << std::endl;
	std::cout << "p2 ? exists  = " << fs::exists(p2) << std::endl;
	std::cout << "p2 ? is File = " << fs::is_regular_file(p2) << std::endl;
	std::cout << "p2 ? is Dir  = " << fs::is_directory(p2) << std::endl;

	fs::path p3 = "/boot/does/not/exist";
	std::cout << "          p3 = " << p3 << std::endl;
	std::cout << "p3.string()  = " << p3.string() << std::endl;
	std::cout << "p3 ? exists  = " << fs::exists(p3) << std::endl;
	std::cout << "p3 ? is File = " << fs::is_regular_file(p3) << std::endl;
	std::cout << "p3 ? is Dir  = " << fs::is_directory(p3) << std::endl;

	
	std::cout << "\n EXPERIMENT 2 ===== Listing directory /etc =====" << std::endl;
	// Show first 10 files of directory /etc 
	dotimes(10, fs::directory_iterator("/etc"),
			[](auto p){
				auto path = p->path();
				std::cout << std::left
						  << std::setw(0) << path.filename().string()
						  << " " << std::setw(35)
						  << std::right << std::setw(40) << path
						  << std::endl;				
			});
	
	std::cout << "\n EXPERIMENT 3 = Listing directory /etc (recursive) =====" << std::endl;
	dotimes(20, fs::recursive_directory_iterator("/etc/"),
			[](auto p){
				std::cout << std::right
						  << std::setw(10)  << fs::is_directory(p->path())
						  << std::setw(10)  << fs::is_regular_file(p->path())
						  << std::setw(10)  << fs::is_symlink(p->path())
						  << std::setw(10)  << " "
						  << std::setw(5)   << std::left << p->path()					  
						  << std::endl;
			});
	
	return EXIT_SUCCESS;
}
