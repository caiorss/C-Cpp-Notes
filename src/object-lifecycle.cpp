// File:  object-lifecycle.cpp 
// Brief: Demonstrate Object Lifecycle 
#include <iostream>
#include <string>

#define DEBUG_TRACE

#ifdef DEBUG_TRACE
  #pragma message "Logging Enabled"
  #define TRACE(msg) \
    std::cerr << __FILE__ << ":" << __LINE__ << ": - fun = " << __FUNCTION__ << " ; " << msg << "\n"
#else
  #pragma message ("Logging disabled")
  #define TRACE(msg)
#endif 

class DummyClass{
public:
        // Constructor 
        DummyClass(const std::string& name): _object_name(name){
                TRACE(std::string("name = ") + name + " - I was created.");
        }
        // Copy constructor 
        DummyClass(const DummyClass& rhs){
                TRACE("Enter copy constructor");
                TRACE("name = " + _object_name + " - I was copied. ");
                this->_object_name = rhs._object_name + "-COPIED";
        }
        // Move constructor 
        DummyClass( DummyClass&& rhs){
                TRACE("Enter move constructor");
                TRACE("name = " + _object_name + " - I was moved");
                this->_object_name = rhs._object_name + "-MOVED";
        }
        // Copy assignment operator
        DummyClass& operator= (const DummyClass& rhs){
                TRACE("Enter copy assignment operator");
                TRACE("name = " + _object_name + " - I was copied. ");
                this->_object_name = rhs._object_name + "-COPIED";
                return *this;
        }
        //DummyClass& operator= (DummyClass&& rhs) = delete;

    // Move assignment operator 
        DummyClass& operator= (DummyClass&& rhs){
                TRACE("Enter move assignment operator");
                TRACE("name = " + _object_name + " - I was moved. ");
                this->_object_name = rhs._object_name + "-MOVED";
                return *this;
        }

        // Destructor 
        ~DummyClass(){
                TRACE(std::string("name = ") + _object_name  + " - I was destroyed");
        }
        void speakWithUser(){
                std::cout << "I am a dummy object called = " << _object_name << "\n";
        }

private:
        std::string _object_name;	
};

// Object allocated on the static memory 
// is deleted when the programs finishes. 
DummyClass dummyGlobal("dummy-global");

auto testObject() -> DummyClass {
        std::cerr << "\n" << " ==> ENTER FUNCTION  testObject()" << "\n\n";
		TRACE("Enter function");
        auto d  = DummyClass("local-dummy-in-function");
        d.speakWithUser();
        std::cerr << "\n" << " ==> EXIT FUNCTION  testObject()" << "\n\n";
		TRACE("Exit function");
        // Object d is deleted here when it goes out scope
        // and then a copy of it is returned from here.
        // Therefore, the copy constructor is invoked.
        return d;
}

auto makeDummyHeap() -> DummyClass* {
        std::cerr << "\n" << " ==> ENTER FUNCTION  makeDummyHeap()" << "\n\n";	
        // Object allocated in dynamic memory, so it survives this scope
        // and is not deleted when returned from function. 
        DummyClass* ptr = new DummyClass("dummy-heap");
        ptr->speakWithUser();
        std::cerr << "\n" << " ==> EXIT FUNCTION  makeDummyHeap()" << "\n\n";
        return ptr; 
};

int main(){
        std::cerr << "\n" << "ENTER FUNCTION MAIN" << "\n\n";
        TRACE("Main function started.");
        // Object allocated on the stack -> auto storage class, it is
        // destroyed when it goes out of scope 
        DummyClass dummy1("dummy1-stack");
        dummy1.speakWithUser();

        DummyClass* dummyInHeap =  makeDummyHeap();
        dummyInHeap->speakWithUser();

        {
                std::cerr << "\n" << " ---- ENTER LOCAL SCOPE " << "\n\n";
                TRACE("Create local scope");
                DummyClass dummy2("dummy2-stack-local-scope");
                dummy2.speakWithUser();
                dummyGlobal.speakWithUser();
                dummyInHeap->speakWithUser();
                TRACE("End local scope");
                // Object dummy2 deleted here
                std::cerr << "\n" << "EXIT LOCAL SCOPE " << "\n\n";
        }

        try {
                std::cerr << "\n" << " ---- ENTER LOCAL EXCEPTION SCOPE " << "\n\n";
                DummyClass dummyException("dummy2-stack-local-scope");
                dummyException.speakWithUser();
                throw std::runtime_error(" ERROR Throw a failure for testing deterministic destructor");
                std::cerr << "\n" << " ---- EXIT LOCAL EXCEPTION SCOPE " << "\n\n";
        } catch (const std::runtime_error& ex){
                std::cerr << "\n" << " ---- ENTER EXCEPTION HANDLER" << "\n\n";		
                std::cerr << "Failure = " << ex.what() << "\n";
                std::cerr << "\n" << " ---- EXIT EXCEPTION HANDLER" << "\n\n";
        }


        TRACE("Copy object returned from function");
        DummyClass dummy2 = testObject();
        dummy2.speakWithUser();

        dummyInHeap->speakWithUser();

        // Objects allocated on the heap must be released manually or a
        // memory leak will happen. However, it is easy to forget to
        // delete a heap-allocated object, so the this approach is error
        // prone and better solution is to use C++11 smart pointers.
        delete dummyInHeap;

        std::cerr << "\n" << "EXIT FUNCTION MAIN" << "\n\n";
        TRACE("Main function finished.");
        return 0;
        // Object dummy1 and dummyGlobal deleted here 
}
