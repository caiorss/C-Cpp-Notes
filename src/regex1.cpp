#include <iostream>
#include <string>

#include <regex>

#define DBG_DISP(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  std::endl

int main()
{
	std::cout << std::boolalpha;
	std::cerr << std::boolalpha;	

	{
		std::puts("\n ======== EXPERIMENT 1 === Match entire string =====");
		std::regex numRegex("\\d+");		
		DBG_DISP(std::regex_match("3423", numRegex));
		DBG_DISP(std::regex_match("AAB786", numRegex));
		DBG_DISP(std::regex_match("  9745", numRegex));
		DBG_DISP(std::regex_match(" 634 ", numRegex));
	}
	{
		std::puts("\n ======== EXPERIMENT 2 === Check whether text has regex =====");
		std::regex variable("\\s?[a-zA-Z][a-zA-Z|\\d]*\\b");
		DBG_DISP(std::regex_search("3423", variable));
		DBG_DISP(std::regex_search("dot.com\\ -buble hello variable4561 ", variable));
		DBG_DISP(std::regex_search("  -dot.com.buble.burst ", variable));
	}
	{
		std::puts("\n ======== EXPERIMENT 3 === Extract text =====");
		std::regex phoneRe{"phone:\\s*(\\d{8})\\s*$"};
		std::string line;
		std::smatch result;
		
		line = "phone:85619751";
		if(std::regex_search(line, result, phoneRe) && result.size() > 1)
			std::cout << "Result1 = " << result.str(1) << "\n";
		else
			std::cout << "Result1 = <<NOT MATCH>>" << std::endl;

		line = "phone:  65658741  ";
		if(std::regex_search(line, result, phoneRe) && result.size() > 1)
			std::cout << "Result2 = " << result.str(1) << "\n";
		else
			std::cout << "Result2 = <<NOT MATCH>>" << std::endl;
		
		line = "phone:  756596213434";
		if(std::regex_search(line, result, phoneRe) && result.size() > 1)
			std::cout << "Result3 = " << result.str(1) << "\n";
		else
			std::cout << "Result3 = <<NOT MATCH>>" << std::endl;

		line = "phone:  ABBX621";
		if(std::regex_search(line, result, phoneRe) && result.size() > 1)
			std::cout << "Result4 = " << result.str(1) << "\n";
		else
			std::cout << "Result4 = <<NOT MATCH>>" << std::endl;		
	}

	{
		std::puts("\n ======== EXPERIMENT 4-A === Finding all regex matches  ======");		
		std::regex nregex("[0-9]+\\b", std::regex::ECMAScript);	
		std::string line = " 565 923 100 -934  AABB56 0835";
		std::sregex_iterator it_beg(line.begin(), line.end(), nregex);
		std::sregex_iterator it_end;
		std::cout << " Number of matches = " << std::distance(it_beg, it_end) << "\n";
		int i = 0;
		for(auto it  = it_beg; it != it_end; it++){ 
			std::cout << " => x[" << i++ << "] = " << it->str() << "\n";
		}
	}
	{
		std::puts("\n ======== EXPERIMENT 4-B === Finding all regex matches  ======");
		// Matches 4 letters followed by 3 digits
		// first capture group 4 letters set, second capture group: 3 digits set 
		std::regex re("([A-Z]{4})([0-9]{3})");	
		std::string line = " ABCD367 XYAZ561 baa 341 MNPQ456 ";
		std::sregex_iterator begin(line.begin(), line.end(), re);
		std::sregex_iterator end;
		for(auto it  = begin; it != end; it++)
			std::cout << " capture[0] = "   << it->str() 
					  << " ; capture[1] = " << it->str(1)
					  << " ; capture[2] = " << it->str(2)
					  << "\n";
	}
	{
		std::puts("\n ======== EXPERIMENT 4-C === Finding all regex matches range-based loop ===");
		// Matches 4 letters followed by 3 digits
		// first capture group 4 letters set, second capture group: 3 digits set 
		std::regex re("([A-Z]{4})([0-9]{3})");	
		std::string line = " ABCD367 XYAZ561 baa 341 MNPQ456 ";
		std::smatch matches;
		std::regex_search(line, matches, re);
		for(const auto& m : matches )
			std::cout << " =>>> "   << m << "\n";
	}	
	{
		std::puts("\n ======== EXPERIMENT 5 === Replace matches  ======");
		// Matches 4 letters followed by 3 digits
		// first capture group 4 letters set, second capture group: 3 digits set 
		std::regex re("([A-Z]{4})([0-9]{3})");	
		std::string line = " ABCD367 XYAZ561 baa 341 MNPQ456 ";
		std::string out = std::regex_replace(line, re, "{$1=$2} - ");
		std::cout << " out = " << out << std::endl;
		
	}	
	return 0;
}
