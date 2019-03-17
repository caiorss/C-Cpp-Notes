//------------------------------------------------------------------------
// Author: Caio Rodrigues
// Brief:  Sample logging system for shared memory using boost interprocess 
//-----------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>

#include <cstring> // strok 
#include <ctime>
#include <chrono>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace bi = boost::interprocess;

int main(int argc, char** argv)
{	
	// Create shared memory wrapper object 
	auto shm = bi::shared_memory_object{
		bi::open_or_create,  
		"logger_shm",
		bi::read_write
	};
    // Set size of the shared memory segment in Kb (kbytes = 1024 bytes)
	shm.truncate(4096); // 4kb (kbytes)
	// Map the shared memory segment to current process
	auto region = bi::mapped_region{shm, bi::read_write};
	// Pointer to shared memory 
	void* pMem = region.get_address();
	
	// Create mutex Object 
    bi::named_mutex amutex(bi::open_or_create, "logger_mutex");
		
	if(argc < 2){
		std::cout << "$ " << argv[0] << " [-client|-server]" << "\n";
		return EXIT_SUCCESS;
	}

	std::string cmd = argv[1];

	if(cmd == "-server"){
		char* pMessage = (char*) pMem;
		while(true)
		{
			std::cerr << " [TRACE] Waiting logging message " << std::endl;
			// Acquire mutex lock blocking this thread 
			amutex.lock();
			// Print logging memssage from shared memory 
			std::cout << pMessage << std::endl;
		}
		return EXIT_SUCCESS;
	}

	if(cmd == "-client")
	{
		// Interpret shared memory as a pointer an null-terminated
		// array of characters 
		char* pMessage = (char*) pMem;
		std::string line;
		while(true)
		{
			std::cout << " => Enter line: ";
			std::getline(std::cin, line);			
			std::stringstream ss;
			auto now = std::chrono::system_clock::now();
			std::time_t ttp = std::chrono::system_clock::to_time_t(now);
			ss << " [INFO] " << strtok(std::ctime(&ttp), "\n") << " - " << line;
			// Print to shared memory (pointer by pMessage)
			std::sprintf(pMessage, "%s", ss.str().c_str());
			// Release mutex lock 
			amutex.unlock();
		}	   
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}
