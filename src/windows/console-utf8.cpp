// File:   console-utf8.cpp 
// Brief:  Sets Windows Console to UTF8 
// Author: Caio Rodrigues 
//-------------------------------------------------------------
#include <iostream>
#include <string>

#include <windows.h>

// Uses RAII for setting console to UTF8
// and restoring its previous settings.
class ConsoleUTF8{
public:
	// Constructor saves context 
	ConsoleUTF8(){
		m_config = ::GetConsoleOutputCP();
		::SetConsoleOutputCP(CP_UTF8);
		std::perror(" [TRACE] Console set to UTF8");
	}
	// Destructor restores context 
	~ConsoleUTF8(){
		std::perror(" [TRACE] Console restored.");
		::SetConsoleOutputCP(m_config);
	}
private:
	unsigned int m_config;
};

// RAII for asking user to hit RETURN to exit
// to block program from exiting immediately when
// the executable is clicked.
struct ExitPrompt{
	~ExitPrompt(){
		std::puts(" >>> Enter RETURN to exit");
		std::cin.get();
	}
};

int main(){
	#ifdef UTF8
	auto utf8Console = ConsoleUTF8();
	#endif 
	//auto codePage = ::SetConsoleOutputCP(CP_UTF8);

	auto exitPrompt = ExitPrompt();
	std::puts("Testing Console output with UTF8");
	std::puts("--------------------------------");
	
	std::puts("Text with UTF8 - SÃO JOÃO -  ");
	std::puts("Japanese Kanji - 漢字 ; Japanese Hiragrama - 平仮名 ");
	std::puts("Cyrllic Script - Sputnik = Спутник-1");
	std::puts("Greek/Latin Script = Α α, Β β, Γ γ, Δ δ, Ε ε, Ζ ζ, Η η, Θ θ");
	
	//::SetConsoleOutputCP(codePage);
	return 0;
}

