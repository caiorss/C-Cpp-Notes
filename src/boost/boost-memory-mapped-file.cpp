#include <iostream>
#include <functional>
#include <string>
#include <fstream>
#include <vector>

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace bi = boost::interprocess;	

/** @brief Returns true if exists. */
bool fileExists(std::string const& fileName)
{
	auto fs = std::ifstream(fileName);
	return fs.good();
}

/** Generic Memory-mapped file allocator */
template<typename T>
using MMFAllocator = bi::allocator<T, bi::managed_mapped_file::segment_manager> ;

/** Generic STL vector allocated in memory-mapped file */
template<typename T>
using MMFVector = std::vector<T, MMFAllocator<T>> ;

int main()
{	
	constexpr const char* fileName = "memory-dump.dat";
	constexpr size_t      fileSize = 4096; // 4 kbytes
	bool flagFileExists            = fileExists(fileName);

	// Manged file mapping object => Creates the file if it does not exists 
	auto mf = bi::managed_mapped_file{bi::open_or_create, fileName, fileSize};

	if(!flagFileExists){
		// Executed when the file did not exist 
		std::cout << " [INFO] Setting file data" << std::endl;
		mf.construct<int>("NumberOfNodes")(100);	
		mf.construct<double>("Speed")(200.0);
		mf.construct<const char*>("Text")("'Allocated text in memory mapped file'");

		// Allocate std::vector<double>
		MMFAllocator<double> aloc1(mf.get_segment_manager());
		MMFVector<double>* pVector = mf.construct<MMFVector<double>>("AVector")(aloc1);
		pVector->reserve(20);
		pVector->push_back(40.5);
		pVector->push_back(98.10);
		pVector->push_back(-50.45);
		pVector->push_back(10);		
		
		return EXIT_SUCCESS;
	}
	
	// ======= Executed when file already exists =========// 
	
	std::cout << " [INFO] Retrieving objects from memory mapped file" << std::endl;

	// Retrieve variable NumberOfNodes with very explicitly and verbose notation 
	std::pair<int*, size_t> pairResult1 =  mf.find<int>("NumberOfNodes");
	if(pairResult1.first == nullptr){
		std::cerr << " [ERROR] I cannot find the object 'NumberOfNodes'" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Number of nodes = " << *pairResult1.first  << "\n";
	(*pairResult1.first)++;

	// Retrieve variable text
	auto [pText, _size1 ] = mf.find<const char*>("Text");
	assert(pText != nullptr);
	std::cout << "Text = " << *pText << "\n";
	
	// Retrieve variable speed
	auto [pSpeed, _size2 ] = mf.find<double>("Speed");
	assert(pSpeed != nullptr);
	std::cout << "Speed = " << *pSpeed << "\n";
	std::cout << " => Set new speed := ";
	std::cin >> *pSpeed;

	// Rerieve vector
	auto [pVector, _size3] = mf.find<MMFVector<double>>("AVector");
	assert(pVector != nullptr);
	std::cout << "\n pVector->size() = " << pVector->size() << std::endl;
	size_t idx = 0;
	for(auto const& x: *pVector)
		std::cout << " pVector[" << idx++ << "] = " << x << std::endl ;	
	
	pVector->push_back(*pSpeed);	
	return 0;
}
