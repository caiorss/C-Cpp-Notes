open System 
open System.Runtime.InteropServices

/// Linux LibC FFI
///
module Libc =
    let x = 10

    //  int puts(const char *s);
    [<DllImport("libc.so.6", EntryPoint="puts")>]
    extern void puts(string)

    // int gethostname(char *name, size_t len);
    [<DllImport("libc.so.6", EntryPoint="gethostname")>]
    extern int private c_getHostname(System.Text.StringBuilder, int len)
    let getHostName () =
        let sb = new System.Text.StringBuilder(64)
        c_getHostname(sb, 64)
        sb.ToString()    

    // double cbrt(double x);
    [<DllImport("libm.so.6", EntryPoint="cbrt")>]
    extern double cbrt(double)
    



[<DllImport("hpc.so", EntryPoint="vector2DNorm")>]
extern double vector2DNorm(double a, double b)

[<DllImport("hpc.so", EntryPoint="arrayNorm")>]
extern double arrayNorm(double [], int)

[<DllImport("hpc.so", EntryPoint="vectorNorm")>]
extern double vectorNorm(double [], int)


[<DllImport("hpc.so", EntryPoint="genArray")>]
extern IntPtr c_genArray(int)

[<DllImport("hpc.so", EntryPoint="genArray2")>]
extern void c_genArray2(double [], int)


let genArray n =
    let ptr = c_genArray n
    let arr = Array.create n 0.0
    Marshal.Copy(ptr, arr, 0, n)
    arr

let genArray2 n =
    let arr = Array.create n 0.0
    let ptr = c_genArray2(arr, n)
    arr


module Circle =
    
    // The accessiblity qualifier "private" makes the function
    // circleNew private to module
    //
    [<DllImport("hpc.so", EntryPoint="Circle_new")>]
    extern void* private circleNew(double radius)

    [<DllImport("hpc.so", EntryPoint="Circle_delete")>]
    extern void private circleDelete(void*)

    [<DllImport("hpc.so", EntryPoint="Circle_getArea")>]
    extern double private circleGetArea(void*)

    let getArea r =
        let circle = circleNew(r)
        let area   = circleGetArea(circle)
        circleDelete(circle)
        area 


let computeArrayNorm() =
    let arr = [| 1.0; 2.0; 3.0; 4.0 |]
    arrayNorm(arr, arr.Length)

let arrayNormWrapper (xs: float []): float =
    arrayNorm(xs, xs.Length)


let testVectorNorm() =
    let v = [| 1.0 ; 2.0; 3.0; 4.0 |]
    vectorNorm(v, v.Length)

[<EntryPoint>]
let main(args) =
    printfn "\n\n"
    printfn "getArea 10.0                    = %f"   <| Circle.getArea 10.0
    printfn "getArea [10.0; 20.0 ; 30; 40.0] = %A"   <| List.map Circle.getArea [10.0; 20.0 ; 30.0; 40.0]
    printfn "vector2DNorm(30.0, 40.0)        = %f"   <| vector2DNorm(30.0, 40.0)
    printfn "computeArrayNorm()              = %f"   <| computeArrayNorm()
    printfn "arrayNormWrapper([|1.0; 2.0; 3.0; 4.0 |] = %f" <|  arrayNormWrapper [|1.0; 2.0; 3.0; 4.0 |]
    printfn "testVectorNorm() = %f" <| testVectorNorm()

    printfn "genArray 5 = %A" <| genArray 5
    printfn "genArray2 5 = %A" <| genArray2 5
    
    printfn "Array.map Libc.cbrt  [| 1.0 ; 8.0; 27.0; 64 |] = %A" <| Array.map Libc.cbrt [| 1.0 ; 8.0; 27.0; 64.0|]
    printfn "Libc.getHostName() = %s" <| Libc.getHostName()
    Libc.puts "\n\nHello world!! It works!!"


    0
