#include <stdio.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

gsl_complex* makeCplRect(double x, double y){
  gsl_complex *cpl = malloc(sizeof(gsl_complex));
  (cpl->dat)[0] = x;
  (cpl->dat)[1] = y;
  return  cpl;
}

void* cplRect(double x, double y){
  gsl_complex *cpl = malloc(sizeof(gsl_complex));
  (cpl->dat)[0] = x;
  (cpl->dat)[1] = y;
  return (void *) cpl;
}


// Create a polar number given the radius r and
// and angle theta in radians.
// 
void* cplPolar(double r, double theta){
  gsl_complex* cpl = malloc(sizeof(gsl_complex));
  *cpl = gsl_complex_polar(r, theta);
  return  (void *) cpl;
}

// Convert a void pointer to gsl_complex 
// 
gsl_complex* void2cpl (void* vptr){
  return (gsl_complex *) vptr;
}

// Get real part.
//  
double cplGetReal(void* cplPtr){
  gsl_complex *cpl = (gsl_complex *) cplPtr;
  return (cpl->dat)[0];    
}

// Get imaginary part.
// 
double cplGetImag(void* cplPtr){
  gsl_complex *cpl = (gsl_complex *) cplPtr;
  return (cpl->dat)[1];    
}

// gsl_complex gsl_complex_mul (gsl complex a, gsl complex b)
// 
void* cpl_add(void* cplPtrA, void* cplPtrB){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* bPtr   = void2cpl(cplPtrB);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_add(*aPtr, *bPtr);
  return result;
}

// gsl_complex gsl_complex_sub (gsl complex a, gsl complex b)
// 
void* cpl_sub(void* cplPtrA, void* cplPtrB){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* bPtr   = void2cpl(cplPtrB);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_sub(*aPtr, *bPtr);
  return result;
}


// gsl_complex gsl_complex_mul (gsl complex a, gsl complex b)
// 
void* cpl_mul(void* cplPtrA, void* cplPtrB){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* bPtr   = void2cpl(cplPtrB);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_mul(*aPtr, *bPtr);
  return result;
}

// gsl_complex gsl_complex_div (gsl complex a, gsl complex b)
// 
void* cpl_div(void* cplPtrA, void* cplPtrB){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* bPtr   = void2cpl(cplPtrB);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_div(*aPtr, *bPtr);
  return result;
}

// 
// gsl_complex gsl_complex_add_real (gsl complex a, double x)
//  
void* cpl_add_real(void* cplPtrA, double x){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_add_real(*aPtr, x);
  return result;
}

// gsl_complex gsl_complex_add_real (gsl complex a, double x) 
// 
void* cpl_sub_real(void* cplPtrA, double x){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_sub_real(*aPtr, x);
  return result;
}

// gsl_complex gsl_complex_mul_real (gsl complex a, double x)
// 
void* cpl_mul_real(void* cplPtrA, double x){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_mul_real(*aPtr, x);
  return result;
}

// gsl_complex gsl_complex_mul_real (gsl complex a, double x)
// 
void* cpl_div_real(void* cplPtrA, double x){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_div_real(*aPtr, x);
  return result;
}


/// -------------- Transcendental Complex Functions ------------- //

// gsl_complex_div_real(*aPtr, x);
// 
void* cpl_sqrt(void* cplPtrA){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_sqrt(*aPtr);
  return result;  
}

// gsl_complex_div_real(*aPtr, x);
// 
void* cpl_exp(void* cplPtrA){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_exp(*aPtr);
  return result;  
}

// gsl_complex gsl_complex_log10
//
void* cpl_log10(void* cplPtrA){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_log10(*aPtr);
  return result;  
}

// gsl_complex gsl_complex_pow (gsl complex z, gsl complex a)
// 
void* cpl_pow(void* cplPtrA, void* cplPtrB){
  gsl_complex* aPtr   = void2cpl(cplPtrA);
  gsl_complex* bPtr   = void2cpl(cplPtrB);
  gsl_complex* result = malloc(sizeof(gsl_complex));
  *result = gsl_complex_pow(*aPtr, *bPtr);
  return result;
}



