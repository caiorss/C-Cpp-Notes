//  Author: Caio Rodrigues - caiorss [dot] rodrigues [at] gmail [dot] com 
//  Brief:  Exhustive examples about boost interprocess - shared memory facilities.
//  Examples:
//   + Allocate string in shared memory
//   + Allocate double array 
//   + Allocate multiple type and STL containers.
//---------------------------------------------------------------------

#include <iostream>
#include <cassert>
#include <map>
#include <functional>
#include <vector>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/segment_manager.hpp>

namespace bi = boost::interprocess;

/** Generic Shared Memory Allocator */
template<typename T>
using ShmemAllocator = bi::allocator<T, bi::managed_shared_memory::segment_manager> ;

/** Generic STL vector allocated in shared memory */
template<typename T>
using ShmemVector = std::vector<T, ShmemAllocator<T>> ;

/** RAII Object for removing shared memory segment. */
struct SharedMemoryCleaner
{	
	std::string m_name;
	SharedMemoryCleaner(std::string name): m_name(name){ }

	~SharedMemoryCleaner(){
		namespace bi = boost::interprocess;
		std::cerr << " [INFO] Remove shared memory segment { name = " << m_name << " }" << "\n";
		bi::shared_memory_object::remove(m_name.c_str());
	}
};

int serverA();
int clientA();

int serverB();
int clientB();

int serverC();
int clientC();


int main(int argc, char** argv){
	using DispatchTable = std::map<std::string, std::function<int ()>>;
	DispatchTable table = {
		{"serverA", &serverA},
		{"clientA", &clientA},
		{"serverB", &serverB},
		{"clientB", &clientB},
		{"serverC", &serverC},
		{"clientC", &clientC}
	};

	if(argc < 2) {
		std::cout << " Error: invalid arguments." << std::endl;
		return EXIT_SUCCESS;
	}
	auto it = table.find(argv[1]);
	if(it == table.end()){
		std::cout << " Error: invalid command." << std::endl;
		return EXIT_FAILURE;		
	}
	// Execute function 
	return it->second();			
}

// ------------- Function Implementations -------------------//

// Print string (char array) to shared memory 
int serverA()
{	
	// Create shared memory wrapper object 
	auto shm = bi::shared_memory_object{
		// Creates shared memory segment if it does not exist 
        bi::open_or_create,
        "shared_memory_segment",
        bi::read_write
	};
	// RAII object that removes the segment when out of scope 
	auto shm_remove = SharedMemoryCleaner("shared_memory_segment");	
    // Set size of the shared memory segment in Kb (kbytes = 1024 bytes)
	shm.truncate(1024); // 1kb (kbytes)
    // Map the shared memory segment to current process
	auto region = bi::mapped_region{shm, bi::read_write};
    // Pointer to shared memory 
	void* pMem = region.get_address();

	// Print to shared memory
	char* pChar = static_cast<char*>(pMem);
	std::sprintf(pChar, " ==> String written to shared memory segment");
	
	// Keep the server Alive as the shared memory segment is not 
	// persistent on Windows. 
	std::cout << "Enter RETURN to EXIT " << "\n";
	std::cin.get();
	return EXIT_SUCCESS;
}

// Print string (char array) to shared memory 
int clientA()
{	
	// Create shared memory wrapper object 
	auto shm = bi::shared_memory_object{
		// Creates shared memory segment if it does not exist 
        bi::open_only,
        "shared_memory_segment",
        bi::read_only
	};
	auto region = bi::mapped_region{shm, bi::read_only};
	void* pMem = region.get_address();

	// Interpret shared memory as a pointer to char* 
	char* pMessage = static_cast<char*>(pMem);
	std::cout << " Content of shared memory = " << pMessage << "\n";	
	return EXIT_SUCCESS;
}


/** Allocate double[] array in shared memory */
int serverB()
{
	auto shm = bi::shared_memory_object{	
        bi::open_or_create,
        "shared_memory_segment",
        bi::read_write
	};
	auto shm_remove = SharedMemoryCleaner("shared_memory_segment");
	shm.truncate(1024); // 1kb (kbytes)
	auto region = bi::mapped_region{shm, bi::read_write};
	void* pMem = region.get_address();
	
	// Allocate array of 5 doubles in the shared memory 
	double* arr = new (pMem) double [5];
	arr[0] = 100.34;
	arr[1] = 200.5;
	arr[2] = -5.6;
	arr[3] = 9.10;
	arr[4] = 10.5;

	// Keep the server Alive as the shared memory segment is not 
	// persistent on Windows. 
	std::cout << "Enter RETURN to EXIT " << "\n";
	std::cin.get();
	return EXIT_SUCCESS;
}

/** Retrieve double[] array from shared memory */
int clientB(){
	auto shm = bi::shared_memory_object{
        bi::open_only, 
        "shared_memory_segment",
        bi::read_only 
	};
	auto region = bi::mapped_region{shm, bi::read_only};
	
	void* pMem = region.get_address();
	double* arr = static_cast<double*>(pMem);

	std::cout << "arr[0] = " << arr[0] << "\n";
	std::cout << "arr[1] = " << arr[1] << "\n";
	std::cout << "arr[2] = " << *(arr + 2) << "\n";
	std::cout << "arr[3] = " << *(arr + 3) << "\n";
	std::cout << "arr[4] = " << arr[4] << "\n";
	return EXIT_SUCCESS;
}


// Allocate STL container in shared memory  
int serverC(){
	// Remove shared memory segment if it already exists 
	bi::shared_memory_object::remove("shared_seg");
	
	auto segment = bi::managed_shared_memory(
		bi::open_or_create,
		"shared_seg", // segment name 
		4096              // 4 kbytes 
		);
	auto shm_remove = SharedMemoryCleaner("shared_seg");
	
	// Segment manager pointer 
	// Type: managed_shared_memory::segment_manage* 
	auto segmgr = segment.get_segment_manager();
	segmgr->construct<const char*>("text")("'Hello world shared memory'");
	segmgr->construct<double>("speed")(10.50);
	segmgr->construct<int>("nodes")(100);

	// === Allocate STL Vector in Shared Memory === //	
    // Build shared memory allocator 
	auto aloc = ShmemAllocator<double>(segmgr);

	// Instantiate vector in shared memory 
	ShmemVector<double>* pVector = segmgr->construct<ShmemVector<double>>("avector")(aloc);
	pVector->reserve(50);
	pVector->push_back(4.5);
	pVector->push_back(10.3);
	pVector->push_back(100.50);
	pVector->push_back(20.0);
	
	std::cout << "Enter RETURN to EXIT " << "\n";
	std::cin.get();
	return EXIT_SUCCESS;
};

int clientC(){
	auto segment = bi::managed_shared_memory(
		bi::open_or_create,
		"shared_seg", // segment name 
		4096          // 4 kbytes 
		);	
	
	auto segmgr = segment.get_segment_manager();
	std::pair<double*, size_t> p1 = segmgr->find<double>("speed");
	std::cout << "Speed = " << *p1.first << "\n";
	
	auto pairNodes = segmgr->find<int>("nodes");
	std::cout << "Nodes = " << *pairNodes.first << "\n";

	auto pairText = segmgr->find<const char*>("text");
	std::cout << "Text = " << *pairText.first << "\n";

	// C++17 - Structured Binding
	auto [pVector, _] = segmgr->find<ShmemVector<double>>("avector");

	std::cout << " => pVector->size() = " << pVector->size() << std::endl;
	std::cout << " => pVector[0] = " << pVector->operator[](0) << std::endl;
	std::cout << " => pVector[1] = " << (*pVector)[1] << std::endl;
	size_t idx = 0;
	for(auto const& x: *pVector)
		std::cout << "pVector[" << idx++ << "] = " <<  x << "\n";
	
	double speed;
	std::cout << "Enter new speed: ";
	std::cin >> speed;
	*p1.first = speed;

	pVector->push_back(speed);
	
	return EXIT_SUCCESS;
};
