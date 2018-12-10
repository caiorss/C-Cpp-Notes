//-----------------------------------------------------------------
// File:   client-socket-shell1.cpp 
// Brief:  Basic Winsocket (Windows socket) echo client shell in C++.
// Author: Caio Rodrigues 
//-----------------------------------------------------------------

#include <iostream>
#include <string>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")

int main(int argc, char** argv){
	if(argc < 3){
		std::cerr << "Usage: " << argv[0] << " [HOSTNAME or ADDRESS] [PORT]" << std::endl;
		return EXIT_FAILURE;
	}

	// Client configuration 
	const char*  host = argv[1];
	unsigned int port = std::stoi(argv[2]);

	//Start up Winsock…
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);
	if(error){
		std::cerr << "Error: failed to initialize WinSock." << std::endl;
		return EXIT_FAILURE;
	}

	// Create client socket 
	SOCKET client = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(client == INVALID_SOCKET){
		std::cerr << "Error, I cannot create socket" << std::endl;
		return EXIT_FAILURE;
	}
	SOCKADDR_IN  addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr(host);
	addr.sin_port        = htons(port);

	// Attemp to connect to server and exit on failure. 
	int connval = connect(client, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if(connval == SOCKET_ERROR){
		std::cerr << "Error: cannot connect to server." << std::endl;
		//Returns status code other than zero
		return EXIT_FAILURE;
	}

	std::string msgClientConnect = " [CLIENT] Connected to server OK.\n";	
	::send(client, msgClientConnect.c_str(), msgClientConnect.size(), 0);

	std::string msg = " [CLIENT SHELL]>> ";
	std::string echo;
	
	bool flag = true;
	while(flag){    
		::send(client, msg.c_str(), msg.size(), 0);
		// Create a buffer with 2024 bytes or 2kb
		std::string buffer(2024, 0);
		//Returns the number of received bytes 
		int n = ::recv(client, &buffer[0], buffer.size()-1, 0);
		echo = " => [ECHO] " + buffer + "\n";
		::send(client, echo.c_str(), echo.size(), 0);
		if(WSAGetLastError() == SOCKET_ERROR){
			std::cerr << "Disconnected OK." << std::endl;
			flag  = false;
			break;
		} else {
			buffer.resize(n);
			if(buffer == "exit\n" ){
				std::string exitMessage = " [CLIENT] Disconnect gracefully - OK.\n";
				::send(client, exitMessage.c_str(), exitMessage.size(), 0);				
				std::cerr << "[LOG] I got an exit message. Shutdowing socket now!" << "\n";
				std::cerr << "[LOG] Gracefully disconnecting application" << "\n";
				std::cerr.flush();
				break;
			}        
			std::cout << " [SERVER SENT] >> " << buffer ; //<< std::endl;
			// Force writing buffer to the stdout
			std::cout.flush();
		}
	}
	std::cerr << "Finished." << std::endl;   
	::closesocket(client);
	::WSACleanup();    
	// Returns status code 0
	return EXIT_SUCCESS;
}
