//
//  GNU - F# Wrapper for Scientific Library 
//
//  Manual: https://www.gnu.org/software/gsl/manual/gsl-ref.html
//
//
open System
open System.Runtime.InteropServices

module Basic =
    // double gsl_expm1 (const double x)
    [<DllImport("libgsl.so", EntryPoint="gsl_log1p")>]
    extern double log1p(double)

    // double gsl_hypot (const double x, const double y)
    [<DllImport("libgsl.so", EntryPoint="gsl_hypot")>]
    extern double hypot(double, double)


module Poly = 
    // This function evaluates a polynomial with real coefficients
    // for the real variable x.
    //   
    // double gsl_poly_eval (const double c[], const int len, const double x)
    [<DllImport("libgsl.so", EntryPoint="gsl_poly_eval")>]
    extern double gsl_poly_eval(double [], int len, double x)

    let polyEval poly x =
        let c = Array.ofList poly
        gsl_poly_eval(c, c.Length, x)

    //  
    //  int gsl_poly_solve_quadratic (double a, double b, double c, double * x0, double * x1)
    //
    // The number of real roots (either zero, one or two) is returned, and
    // their locations are stored in x0 and x1. If no real roots are found
    // then x0 and x1 are not modified. If one real root is found (i.e. if
    // a=0) then it is stored in x0. When two real roots are found they are
    // stored in x0 and x1 in ascending order. The case of coincident roots
    // is not considered special. For example (x-1)^2=0 will have two roots,
    // which happen to have exactly equal values.  
    [<DllImport("libgsl.so", EntryPoint="gsl_poly_solve_quadratic")>]
    extern int private gsl_poly_solve_quadratic(double a,
                                                double b,
                                                double c,
                                                double& x1,
                                                double& x2)

    /// This function finds the real roots of the quadratic equation,
    //   a x^2 + b x + c = 0
    ///
    let solveQuadratic a b c =
        let mutable x0 =  0.0
        let mutable x1 =  0.0 
        let nroots = gsl_poly_solve_quadratic(a, b, c, &x0, &x1)
        match nroots with
        | 0 -> []
        | 1 -> [x0]
        | 2 -> [x0 ; x1]
        | _ -> failwith "Error: Invalid number of roots returned"

    ///  int gsl_poly_solve_cubic (double a, double b, double c, double * x0, double * x1, double * x2)
    ///
    ///  This function finds the real roots of the cubic equation, x^3 + a x^2 + b x + c = 0
    ///
    [<DllImport("libgsl.so", EntryPoint="gsl_poly_solve_quadratic")>]
    extern int private gsl_poly_solve_cubic(double a,
                                            double b,
                                            double c,
                                            double& x1,
                                            double& x2,
                                            double& x3)

    ///  This function finds the real roots of the cubic equation,
    ///  x^3 + a x^2 + b x + c = 0
    ///
    let solveCubic a b c =
        let mutable x0 =  0.0
        let mutable x1 =  0.0
        let mutable x2 =  0.0 
        let nroots = gsl_poly_solve_cubic(a, b, c, &x0, &x1, &x2)
        match nroots with
        | 0 -> []
        | 1 -> [x0]
        | 3 -> [x0 ; x1 ; x2]
        | _ -> failwith "Error: Invalid number of roots returned"


module Bessel =

    /// These routines compute the regular cylindrical Bessel function
    /// of zeroth order, J_0(x).
    ///
    /// double gsl_sf_bessel_J0 (double x)
    [<DllImport("libgsl.so", EntryPoint="gsl_sf_bessel_J0")>]
    extern double besselJ0(double x)

    /// These routines compute the regular cylindrical Bessel function o
    ///
    ///  double gsl_sf_bessel_Jn (int n, double x)
    ///
    [<DllImport("libgsl.so", EntryPoint="gsl_sf_bessel_Jn")>]
    extern double besselJn(int n, double x)
    



(*  C-struct equivalent.
    ---------------------
                          
        typedef struct
        {
          double dat[2];
        } gsl_complex;
*)
[<Struct>]
[<StructLayout(LayoutKind.Sequential)>]
type gsl_complex =
    struct

        //[<MarshalAs(UnmanagedType.LPArray, SizeConst=2)>]
        [<MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)>]
        val dat: double []
    end


let gslCplToTuple (cpl: gsl_complex) =
    (cpl.dat.[0], cpl.dat.[1])

//  This function uses the rectangular Cartesian components (x,y) to
//  return the complex number z = x + i y. An inline version of this
//  function is used when HAVE_INLINE is defined.
//
[<DllImport("libgsl.so"
           ,EntryPoint="gsl_complex_rect"
           ,CallingConvention=CallingConvention.Cdecl)>]
extern IntPtr gsl_complex_rect(double x, double Y)
    

let ptrToCpl (p: IntPtr): gsl_complex =
    Marshal.PtrToStructure(p)

let complexRect(x, y) =
    gsl_complex_rect(x, y) |> ptrToCpl


