/*  File:   test_terminate.cpp 
 *  Author: Caio Rodrigues 
 *  Brief:  Override std::termiante_handler for understanding what happens during abnormal program termination.
 *----------------------------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

class DummyClass{
public:
    DummyClass()
    {
        std::puts(" [INFO] Object ctor => Constructor called.");
    }
    ~DummyClass()
    {
        std::puts(" [INFO] Object dtor => Destructor called.");
    }
};


int main(int argc, char** argv)
{
    DummyClass cls;

    const char* option = std::getenv("TERMINATE");
    if(option != nullptr && std::string(option) == "true")
    {

        std::puts(" [TRACE] Override std::terminate handler.");
        // Overrides the function called by std::terminate (std::abort by default).
        //
        // It accepts function pointer or a non-capturing lambda.
        // Override std::termiante setting std::terminate_handler callback
        std::set_terminate([](){
            std::cerr << " [FATAL] std::terminate() called; SIGABRT signal abort sent."
                      << std::endl;
            // terminate current process sending abrt
            std::abort();
        });
    }

    // Register function to be called when the program exits.
    // Note: It does not override the normal termination behavior.
    std::atexit([](){
        std::cerr  << " [INFO] Normal termination. Ok." << std::endl;
    });


    if(argc < 2) {
        std::puts(" [ERROR] Missing command. Shutdown");
        return 1;
    }
    std::string cmd = argv[1];

    if(cmd == "terminate_normal1")
    {
        std::puts(" [TRACE] Nothing.");
    }
    else if(cmd == "terminate_normal2")
    {
        std::puts(" [TRACE] Call std::exit(int status_code);");
        std::exit(2);
    }
    else if (cmd == "terminate_except")
    {
        std::puts(" [TRACE] Before throwing exception");
        throw std::runtime_error("Invalid input domain");
        std::puts(" [TRACE] After throwing exception");

    } else if (cmd == "terminate_thread")
    {
        using namespace std::chrono_literals;
        std::puts(" [TRACE] Joinable thread out of scope without calling .join() or detach");

        std::thread th{ [](){
                while(true) {
                    std::puts(" [TRACE] Thread running ....");
                    std::this_thread::sleep_for(1s);
                }
        }};

        // Missing thread::join() or thread::detach method call
        // => The runtime calls std::terminate()
    }

    std::cout  << " [TRACE] End of main function" << std::endl;

    return 0;
}
