// Brief: C++ Wrapper example to GNU Scientific Library Vector.
// Author: Caio Rodrigues
// Compile with: $ clang++ gsl-vector-wrapper.cpp -std=c++11 -Wall -Wextra -g -lgsl -lgslcblas -o out.bin
// Run with:     $ ./out.bin
// 
// GNU Scientific Library - C++ Linear Algebra Wrapper
//----------------------------------------------------------------------
#include <iostream>
#include <string>
#include <functional>
#include <cmath>
#include <ostream> 
#include <iomanip>
#include <vector>
#include <cassert>

// Install GNU Scientific Library on Fedora with:
// $ sudo dnf install gsl-devel.x86_64
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_vector.h> 
#include <gsl/gsl_matrix.h> 
#include <gsl/gsl_rng.h> /* Random numbers*/
#include <gsl/gsl_blas.h>


// Load Shared libraries needed by CLING REPL.
#ifdef  __CLING__
  R__LOAD_LIBRARY(/lib64/libgslcblas.so.0);
  R__LOAD_LIBRARY(/lib64/libgsl.so);
#endif 

class GSLVector{
private:
    gsl_vector* m_hnd;
    int m_size;
public:
    GSLVector(int m_size) {
        this->m_size = m_size;
        this->m_hnd = gsl_vector_alloc(m_size);
    }
    // Overloaded constructor
    GSLVector(int m_size, double x) {        
        this->m_size = m_size;
        this->m_hnd = gsl_vector_alloc(m_size);
        gsl_vector_set_all(m_hnd, x);
    }
    // Copy constructor 
    GSLVector(const GSLVector& src){
        m_size = src.m_size;
        m_hnd = gsl_vector_alloc(src.m_size);
        gsl_vector_memcpy(m_hnd, src.m_hnd);
    }
	// Copy assignment operator
    GSLVector& operator= (const GSLVector& src){
		std::cerr << " [TRACE] Copy assignment operator." << "\n";
        if(m_size != src.m_size){
			gsl_vector_free(m_hnd);
			m_hnd = gsl_vector_alloc(src.m_size);
		}        
        gsl_vector_memcpy(m_hnd, src.m_hnd);
		return *this;
    }	
    // Destructor 
    ~GSLVector(){
		if(m_hnd != nullptr)
			gsl_vector_free(m_hnd);
    }
	size_t size() const { return m_size; }

    gsl_vector* data(){
        return this->m_hnd;
    }
	struct ElementProxy{
		GSLVector* ptr;
		size_t     index;
		ElementProxy(GSLVector* ptr, size_t index): ptr(ptr), index(index){}
		ElementProxy& operator=(double x){
			gsl_vector_set(ptr->data(), index, x);
			return *this;
		}
		double get() const {
			return gsl_vector_get(ptr->data(), index);
		}
	};

	ElementProxy operator[](size_t index){
		return ElementProxy(this, index);
        // return gsl_vector_get(m_hnd, index);
    }
    // double operator[](int index){
    //     return gsl_vector_get(m_hnd, index);
    // }
	
    void set(int index, double x){
        gsl_vector_set(m_hnd, index, x);
    }
    double max() const {
        return gsl_vector_max(m_hnd);        
    }
    double min() const {
        return gsl_vector_min(m_hnd);
    }
    GSLVector operator + (const GSLVector& va){
        // Invoke copy constructor 
        GSLVector vec2 = *this;
        gsl_vector_add(vec2.m_hnd, va.m_hnd);
        return vec2;
    }
    GSLVector operator * (double scale){
        // Invoke copy constructor 
        GSLVector vec2 = *this;
        gsl_vector_scale(vec2.m_hnd, scale);
        return vec2;
    }
    friend std::ostream& operator<<(std::ostream& os, const GSLVector& vec){
        os << "[" << vec.m_size << "](";
        for(int i = 0; i < vec.m_size; i++){
            os << std::setprecision(4) << std::fixed << " " << gsl_vector_get(vec.m_hnd, i);
        }
        os << ") ";
        return os;
    }
};

int main(){
	GSLVector vec1(5, 2.45);
	vec1.set(0, -3.45);

	std::cout << std::boolalpha;
	std::cout << "vec1 = " << vec1 << "\n";
	// test copy constructor
	std::puts("Create vec2 - before invoke copy constructor");
	GSLVector vec2 = vec1;
	std::cout << "vec1 = " << vec2 << "\n";
	std::cout << "&vec2 == &vec1: " << (&vec1 == &vec2) << "\n";
	assert(&vec2 != &vec1);
	vec2.set(0, 10.0);
    vec2.set(1, 25.0);
	vec2[3] = 2.30;
	std::cout << "vec2 = " << vec2 << "\n";
	
	GSLVector vec3 = vec1 + vec2;
	std::cout << "vec3 = " << vec3 << "\n";
	std::cout << "vec1 * 3 + vec2 * 2.5 = " << vec1 * 1.5 + vec2 * 2.5 << "\n";
	
	return 0;
}

