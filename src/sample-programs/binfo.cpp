// File:   binfo.cpp 
// Brief:  Get information about binary files and identify them using the "magic numbers".
// Author: Caio Rodrigues 
// File signature or magic number database: https://gist.github.com/overtrue/0a2aec7c2fbe9621a869
// C++ Standard: >= C++14
//----------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstring>

using byte = std::uint8_t;
using ByteArray = std::vector<byte>;

// ====> Template which only works with float points ----------//
template<class T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
readFromStream(std::istream& is){
	T t{};
	is.read((char*) &t, sizeof(T));	
	std::cout << " RESULT ==> "  << t << "\n";
}

// ====> Template which only works with integers ----------//
template<class T>
typename std::enable_if<std::is_integral<T>::value, void>::type
readFromStream(std::istream& is){
	T t{};
	is.read((char*) &t, sizeof(T));
	if(typeid(T) == typeid(uint8_t)){
		std::cout << " RESULT ==> {"
				  << "hex = 0x" << std::hex << std::uppercase << static_cast<int>(t)
				  << " ; dec = " << std::dec << static_cast<int>(t)
				  << "} \n";
		return;
	}
	std::cout << " RESULT ==> {"
			  << "hex = 0x" << std::hex << std::uppercase << t
			  << " ; dec = " << std::dec << t << "} \n";
}

/** Print byte array as string and non-printable chars as hexadecimal */
auto ByteArrayToString(const ByteArray& str) -> std::string {
	auto os = std::stringstream{};
	for(const auto& ch: str)
		if(std::isprint(ch))
			os << ch << " ";
		else
			os << "\\0x" << std::hex << std::setw(2) << std::setfill('0')
			   << static_cast<int>(ch) << " "
			   << std::dec;
	return os.str();
}

auto ByteArrayToHexString(const ByteArray& str) -> std::string {
	auto os = std::stringstream{};
	for(const auto& ch: str)
		os << std::hex << std::setw(2) << std::setfill('0')
		   << static_cast<int>(ch) << " "
		   << std::dec;
	return os.str();
}

// Attempt to turn character into string if character is printable
// otherwise, returns the hexadecimal representation.
//-------------------------------------------------------------
auto printChar(char ch) -> std::string;

int main(int argc, char** argv){
	extern std::map<ByteArray, std::string> SignatureMap;
	
	auto showUsage =
		[]{
			std::puts("Binary Info => Extract information from binary files.");
			std::puts("Usage: ./binreader info       [FILE]");
			std::puts("Usage: ./binreader get        [TYPE]  [OFFSET] [FILE]");
			std::puts("Usage: ./binreader bytes-char [SIZE]  [OFFSET] [FILE]");
			std::puts("Usage: ./binreader bytes-hex  [SIZE]  [OFFSET] [FILE]");						 
		};
	
	auto command = std::string{argv[1]};

    // Print the ascii table 
    if(command == "ascii"){							
		for(int i = 0; i < 128; i++){							
			std::cout << std::setw(5)  << std::right << std::dec << i 
			          << std::setw(8)  << std::right << "0x" << std::hex << i
			          << std::setw(5)  << " "
			          << std::setw(10) << std::left  << printChar(i)
			          << std::endl;
		}
		return EXIT_SUCCESS;
     }

	if(argc < 3){
		showUsage();
		//std::perror("HERE");
		return EXIT_FAILURE;
	}

	if(command == "info"){		
		size_t maxlen = 0;		
		// Get the maximum array size 
		for(const auto& x: SignatureMap){
			maxlen = std::max(maxlen, x.first.size());
		}
		std::printf("Maximum size = %zu\n", maxlen);
		ByteArray bytes(maxlen, 0x00);
		auto file = std::string{argv[2]};
		auto fd   = std::ifstream(file,  std::ios::in | std::ios::binary);
		fd.read((char*) bytes.data(), bytes.size());
		if(fd.fail()){
			std::cerr << "Error: cannot open file " << file << std::endl;
			return EXIT_FAILURE;
		}		
		auto it = std::find_if(
			SignatureMap.begin(), SignatureMap.end(),
			[&bytes]( auto& pair){
				const auto& vec = pair.first;
				return std::equal(vec.begin(), vec.end(), bytes.begin());
			});
		if(it != SignatureMap.end())
			std::cout << it->second << "\n";
		else
			std::cout << "File not identified." << "\n";
		std::cout << "\n Bytes at 0x00 ==> " << ByteArrayToHexString(bytes) << std::endl;
		return EXIT_SUCCESS;
	}
	if(argc != 5){
		showUsage();
		return EXIT_FAILURE;
	}
	
	/** ./binreader get   [TYPE]  [OFFSET]  [FILE] */
	if(command == "get"){
		std::string type  = argv[2];
		size_t      offset = std::stoul(argv[3], nullptr, 16);
		std::string file   = argv[4];
		auto fd = std::ifstream(file,  std::ios::in | std::ios::binary);
		if(fd.fail()){
			std::fprintf(stderr, "Error: could not open file name: %s", file.c_str());
			return EXIT_FAILURE;
		}					
		fd.seekg(offset, std::ios::beg);
		// if(type == "char")   readFromStream<char>(fd);

		// unsigned integer - 1 byte
		if(type == "ui1")    readFromStream<uint8_t>(fd);
		// unsigned integer - 2 bytes 
		if(type == "ui2")    readFromStream<uint16_t>(fd);
		// unsigned integer - 4 bytes 
		if(type == "ui4")    readFromStream<uint32_t>(fd);
		// unsigned integer - 8 bytes 
		if(type == "ui8")    readFromStream<uint64_t>(fd);
		
		if(type == "si1")    readFromStream<int8_t>(fd);
        // signed integer - 2 bytes 
		if(type == "si2")    readFromStream<int16_t>(fd);
		// signed integer - 4 bytes 
		if(type == "si4")    readFromStream<uint32_t>(fd);
		// signed integer - 8 bytes 
		if(type == "si8")    readFromStream<uint64_t>(fd);		

        // --- Float point ----- //
		if(type == "float") readFromStream<float>(fd);
		if(type == "double") readFromStream<double>(fd);
		return EXIT_SUCCESS;
	}

	auto readByteArrayFromFile =
		[](const std::string& file, size_t size, size_t offset) -> ByteArray {
			ByteArray bytes(size, 0x00);
			auto fd = std::ifstream(file,  std::ios::in | std::ios::binary);
			if(fd.fail()){
				std::fprintf(stderr, "Error: could not open file name: %s", file.c_str());
				throw std::runtime_error("Error could not open file");
			}			
			fd.seekg(offset, std::ios::beg);
			fd.read((char*) bytes.data(), bytes.size());
			return bytes;
		};

	/** Usage: ./binreader bytes [SIZE] [OFFSET] [FILE] */
	if(command == "bytes-char"){
		size_t size       = std::stoul(argv[2]);
		size_t offset     = std::stoul(argv[3], nullptr, 16);
		std::string file  = argv[4];
		auto bytes  = readByteArrayFromFile(file, size, offset);
		std::cout << " RESULT ==> " << ByteArrayToString(bytes) << "\n";
		return EXIT_SUCCESS;
	}
	if(command == "bytes-hex"){
		size_t size       = std::stoul(argv[2]);
		size_t offset     = std::stoul(argv[3], nullptr, 16);
		std::string file  = argv[4];
		auto bytes  = readByteArrayFromFile(file, size, offset);
		std::cout << " RESULT ==> " << ByteArrayToHexString(bytes) << "\n";
		return EXIT_SUCCESS;		
	}

	return EXIT_SUCCESS;
} // ------------- End of main() ---------------//


auto printChar(char ch) -> std::string {
	static auto const specialChars = std::map<char, std::string>
	{
			{0x00, "NULL"}, {0x07, "BELL"}, {0x08, "BACKSPACE"}, 
			{0x11, "TAB"},  
			{0x10, "LF - ^A - Line Feed"},
			{0x15, "CR - ^M - Carriage Return"},
			{0x1B, "ESCAPE - ^["}, {0x7F, "DELETE"}
	};
	std::stringstream os;
	if(::isprint(ch)) 
		os << ch;
	else {
		auto it = specialChars.find(ch);
		if(it != specialChars.end())
			return it->second;
		else 
			os << "\\0x" << std::hex << (0xFF & static_cast<int>(ch))
			   << std::dec;
	}
	return os.str();	   
}

//==========>> File Signatures Database =====//

auto SignatureMap = std::map<ByteArray, std::string>{
	//====> Executable formats <<=========
	 {{0x4D, 0x5A},
	 "Windows PE (Portable Executable) native executable or DLL => .exe, .dll, .sys, .oct ..."}
	 ,{{0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x2F, 0x43, 0x2B, 0x2B, 0x20},
	   ".pdb => Visual C++ Compiler Debug Symbols "}
	 ,{{0x43, 0x52, 0x45, 0x47},
	   "Win9X Registry"}
	,{{0xFE, 0xED, 0xFA, 0xCE},
	 "N/A => MachO Executable (32 bits) / Native executable from Apple's MacOSX OS."}
	,{{0xFE, 0xED, 0xFA, 0xCF},
	 "N/A => MachO Executable (64 bits) / Native executable from Apple's MacOSX OS."}	
	,{{0x7F, 0x45, 0x4C, 0x46},
	  ".N/A =>  Unix Executable - ELF"}
	,{{0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x08}, // Needs to be checked.
	  ".jar => Java Package Archive"}
	,{{0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1},
	  ".msi => Microsoft Installer"}
	,{{0xCA, 0xFE, 0xBA, 0xBE},
	  ".class => Java compiled bytecode"}
	,{{0x64, 0x65, 0x78, 0x0A,  0x30, 0x33, 0x35, 0x00},
	  "Dalvik Executable"}   

	//====> Document formats <<==========
	 ,{{0x49, 0x54, 0x53, 0x46},
	   "Windows compressed help file => .chi, .chm"}
	 ,{{0x4C, 0x00, 0x00, 0x00, 0x01, 0x14, 0x02, 0x00},
	   ".lnk => Windows shortcut file"}
	 ,{{0xFF, 0xD8, 0xFF},
	  "JPEG Image - Extensions .jpg, .jpeg, .jpe, .jif, .jfif, .jfi"}
	 ,{{0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF},
	   ".wmv - Windows Video File"}
	 ,{{0x00, 0x00, 0x00, 0x18, 0x66, 0x74, 0x79, 0x70, 0x6D, 0x70, 0x34, 0x32},
	   ".mp4 - Mpge 4 Video File"}
	 ,{{0x00, 0x00, 0x00, 0x14, 0x66, 0x74, 0x79, 0x70, 0x69, 0x73, 0x6F, 0x6D},
	   ".mp4 - MPEG-4 Video File v1"}
	 ,{{0x25, 0x50, 0x44, 0x46, 0x2d},
	   ".pdf PDF - Portable Document File"}   
	 ,{{0x78, 0x01, 0x73, 0x0D, 0x62, 0x62, 0x60},	   
	   ".dmg => Apple Disk Image file"}
	,{{0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x0, 0x1C},
	  ".7z => 7Zip archive file (compressed file.)"}
	 ,{{0x21, 0x3C, 0x61, 0x72, 0x63, 0x68, 0x3E},
	   ".deb => Linux Debian Package file" }
	 ,{{0x43, 0x44, 0x30, 0x30, 0x31},
	   ".iso =>> ISO disk Image file - application/x-iso9660-image"}
	 ,{{0x41, 0x43, 0x31, 0x30},
	   ".iso => Compressed ISO CD image"}
	 ,{{0x53, 0x51, 0x4C, 0x69, 0x74, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x6D, 0x61, 0x74, 0x20, 0x33, 0x00},
	   ".sqlite => SQLite Database"}
	 ,{{0x01, 0x0F, 0x00, 0x00},
	   ".MDF => SQL Database"}
	 ,{{0x0C, 0xED},
	   "MultiBit Bitcoin Wallet."}
	 ,{{0xF9, 0xBE, 0xB4, 0xD9},
	   ".dat => Bitcoin-Qt blockchain block file"}
	 ,{{0x45, 0x86, 0x00, 0x00, 0x06, 0x00},
	   "Quickbooks backup file"}
};
