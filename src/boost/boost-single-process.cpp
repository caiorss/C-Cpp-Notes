#include <iostream>
#include <string> 
#include <functional> 

#include <boost/interprocess/sync/named_mutex.hpp>

class ResourceRAII
{
  using Action = std::function<void ()>;
  const char* m_name{};  
  Action  m_disposer{};
public:
  ResourceRAII(){}
  ResourceRAII(const char* name, Action disposer):
    m_name(name), m_disposer{disposer}
  {
  }
  ~ResourceRAII()
  {
    std::cerr << "Resource " << m_name << " disposed OK" << std::endl;
    m_disposer();
  }
};

int main()
{
  namespace bi = boost::interprocess;

  const char* mutexName = "my-app-mutex";

  // bi::named_mutex::remove(mutexName);
  
  // Create inteprocess mutex Object 
  bi::named_mutex m1(bi::open_or_create, mutexName);
  // Attempts to acquire mutex lock, if it is locked 
  // return true, otherwise returns false.
  bool isLocked = m1.try_lock();

  std::cout << std::boolalpha << " [INFO] isLocked = "
	    << isLocked
	    << std::endl;
  
  ResourceRAII cleaner{};  
  if(isLocked)
  {
    cleaner = ResourceRAII( mutexName,
			      [&]{
				bi::named_mutex::remove(mutexName);
			      });    
  } else {
    std::cerr << " [Error] Process is already running\n";
    return 0;
  }

  std::cout << "Running process ... OK " << std::endl;
  std::cout << "Type RETURN to exit" << std::endl;
  std::cin.get();
    
  return 0; 
}
