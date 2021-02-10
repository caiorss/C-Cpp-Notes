///  File:    new-delete-experiments.cpp
///  Author:  Caio Rodgrigues - caiorss [dot] rodrigues [at] gmail [dot] com
///  Brief:   Operator new and delete experiments
///------------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <cstring>


// ---------------- Testing Class ------------------------------//

class SomeClass{
public:
    int x;
    int y;
    char buffer[200];

    SomeClass(): x(125), y(2561), buffer("Hello world")
    {
        std::puts(" [TRACE] SomeClass object created OK");
    }

    void show() const
    {
        std::printf(" =>> SomeClass { x = %d, y = %d, buffer = %s } \n", x, y, buffer);
    }

    ~SomeClass()
    {
        std::puts(" [TRACE] SomeClass object destroyed OK");
    }
};

class SomeClassB{
    char m_name[200] = "Unnamed";
    int  m_id = -1;

    SomeClassB()
    {
        std::printf(" [TRACE] <CTOR1> Object of SomeClassB created => addr = %p "
                    " id = %d - name = '%s'"
                    , static_cast<void*>(this), m_id, m_name);
    }

    ~SomeClassB()
    {
        std::printf(" [TRACE] <DTOR> Object of SomeClassC deleted => addr = %p \n"
                    , static_cast<void*>(this));
    }

};

//----- Overload new and delete operator as member functions -----//

class SomeClassC {
public:
    char m_name[200] = "Unnamed";
    int  m_id = -1;

    SomeClassC()
    {
        std::printf(" [TRACE] <CTOR1> Object of SomeClassC created => addr = %p "
                    " id = %d - name = '%s'"
                    , static_cast<void*>(this), m_id, m_name);
    }

    SomeClassC(const char* name, int id)
    {
        std::strcpy(m_name, name);
        m_id = id;
        std::printf(" [TRACE] <CTOR2> Object of SomeClassC created => addr = %p "
                    " id = %d - name = '%s' \n"
                    , static_cast<void*>(this), m_id, m_name);
    }

    ~SomeClassC()
    {
        std::printf(" [TRACE] <DTOR> Object of SomeClassC deleted => addr = %p \n"
                    , static_cast<void*>(this));
    }

    // Member fuction overloading of new operator
    // -------------------------------------------------------
    // Note: the keyword static is redundant, this operator is a static member function
    // even without the keyword 'static'. It was added to make the declaration more explicit.
    static void* operator new(size_t sz)
    {
        void* ptr = std::malloc(sz);
        if(!ptr) std::puts(" [ERROR] Not enough memory");
        std::printf(" [TRACE] SomeClassB => Custom new operator called. Allocated %lu Bytes => "
                    " ptr = %p \n"
                    , sz, ptr);
        return ptr;
    }

    // Member fuction overloading of delete operator
    // Static keyword redundant
    static void operator delete(void* addr)
    {
        std::printf(" [TRACE] Custom delete operator called for SomeClassB => p = %p \n"
                    , addr);
        free(addr);
    }

};



// ------- Overloading of global new and delete operators --------//

// Global free-function overloading of new operator
// Note: Cannot be declared inside a namespace
// Note: DO NOT DO IT!
void* operator new(size_t size)
{
    std::printf(" [TRACE] NEW - Called operator new, Allocated %lu Bytes\n", size);
    void* ptr = std::malloc(size);
    if(!ptr) {
        std::puts(" [ERROR] Not enough memory");
        throw std::bad_alloc();
    }
    return ptr;
}


// Global free-function overloading of new operator (std::notrhow)
// Note: Cannot be declared inside a namespace
// Note: Does not throw exception, just returns a null pointer when there is no
//       memory available
void* operator new(size_t size, std::nothrow_t const& tag)
{
    std::printf(" [TRACE] NEW (std::nothrow) - Called operator new, Allocated %lu Bytes\n", size);
    void* ptr = std::malloc(size);
    if(!ptr) {
        std::puts(" [ERROR] Not enough memory");
        return nullptr;
    }
    return ptr;
}

// Global free-function overloading of delete operator
// Note: Cannot be delcared inside a namespace
void operator delete(void* ptr) noexcept
{
    std::puts(" [TRACE] DELETE - Called operator delete");
    free(ptr);
}


int main()
{

    std::puts("\n ==>> Experiment 1 == Global overloading -test with primitive type \n");
    {
        int* ptr = new int(25);
        *ptr = 10 + *ptr;
        std::printf(" Value of ptr[%p] = %d \n", static_cast<void*>(ptr), *ptr);
        delete  ptr;
    }

    std::puts("\n ==>> Experiment 2 == Global Overloading - test with class ====\n");
    {
        SomeClass* pcls = new SomeClass;
        pcls->x = 25;
        pcls->y = 1005;
        pcls->show();
        delete pcls;
    }

    std::puts("\n ==>> Experiment 3 == (std::notrhow) Global Overloading - test with class ====\n");
    {
        SomeClass* pcls = new (std::nothrow) SomeClass;
        pcls->x = 25;
        pcls->y = 1005;
        pcls->show();
        delete pcls;
    }


    std::puts("\n ==>> Experiment 4 - Explicit raw allocation  == Global Overloading - test with class ====\n");
    {
        // Raw memory allocation => Without calling the constructor
        void* raw_pcls = ::operator new(sizeof(SomeClass));
        // Note: The constructor is not called
        SomeClass* pcls = static_cast<SomeClass*>(raw_pcls);
        pcls->x = 251;
        pcls->y = -1005;
        std::strcpy(pcls->buffer, "raw-memory-allocated");
        pcls->show();
        delete pcls;
    }

    std::puts("\n ==>> Experiment 5 === Member Function Overloading ==========\n");
    {
        SomeClassC* pclassB = new SomeClassC("Hello", 2005);
        std::printf(" =>>  Print object => pclassB->m_name = %s; pclassB->m_id = %d \n"
                    , pclassB->m_name, pclassB->m_id);

        delete pclassB;
    }

    return 0;
}
