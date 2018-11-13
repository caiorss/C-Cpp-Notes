// File:   singleton1.cpp 
// Brief:  Singleton design pattern demonstration.  
// Author: Caio Rodrigues 
#include <iostream>
#include <deque>
#include <string> 
#include <memory>

//------ It should be  in file: FileRepository.hpp -----//

// Singleton is a design pattern with a single instance
// It implements a file repository, there can be only one instance
// of this object.
// 
class FileRepository
{
private:
    std::deque<std::string> _files;	
    // Forbid client code instating a new instance. 
    FileRepository(){
		std::cerr << " [LOG] File Respository Initialized." << "\n";
	}	
    // Forbid client code from creating a copy or using the
    // copy constructor.
    FileRepository(const FileRepository&){}		
public:
	~FileRepository();
	
	// Return a reference to not allow client code 
	// to delete object. 	
    static auto getInstance() -> FileRepository&;	

	// Use old C++ 'member function' syntax.
	void addFile(std::string fname);
	void clearFiles();
	// C++11 member function declaration looks better. 
    auto showFiles() -> void;
};

//------ It should be  in file: FileRepository.cpp -----//

FileRepository::~FileRepository(){
	std::cerr << " [LOG] File Respository Deleted. Ok." << "\n";
}

// Static method
auto FileRepository::getInstance() -> FileRepository& {
	static auto instance = std::unique_ptr<FileRepository>{nullptr};		
	// Initialized once - lazy initialization 
	if(!instance)
		instance.reset(new FileRepository);		
	return *instance.get();
}

void FileRepository::addFile(std::string fname){
	_files.push_back(std::move(fname));
}

void FileRepository::clearFiles(){
	_files.clear();
}

auto FileRepository::showFiles() -> void {
	for(const auto& file: _files)
		std::cout << " File = " << file << std::endl;
}


//---- It should be file: main.cpp -------------//

int main(){

    FileRepository& repo1 = FileRepository::getInstance();
	repo1.addFile("CashFlowStatement.txt");
	repo1.addFile("Balance-Sheet.dat");
	repo1.addFile("Sales-Report.csv");

	FileRepository& repo2 = FileRepository::getInstance();
	
	std::cout << std::boolalpha << "Same object? (&repo == &repo1 ?) = "
			  << (&repo1 == &repo2)
			  << "\n";
	std::cout << "Repository files" << std::endl;
	repo2.showFiles();

	std::cout << "Add more files" << std::endl;
	repo2.addFile("fileX1.pdf");
	repo2.addFile("fileX2.pdf");
	repo2.addFile("fileX3.pdf");
	repo2.showFiles();
	
	return EXIT_SUCCESS;
}
