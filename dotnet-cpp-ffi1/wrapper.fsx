open System 
open System.Runtime.InteropServices



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

[<DllImport("hpc.so")>]
extern double vectorSum(double [], int)


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
    printfn "getArea 10.0                                   = %f" <| Circle.getArea 10.0
    printfn "getArea [10.0; 20.0 ; 30; 40.0]                = %A" <| List.map Circle.getArea [10.0; 20.0 ; 30.0; 40.0]
    printfn "vector2DNorm(30.0, 40.0)                       = %f" <| vector2DNorm(30.0, 40.0)
    printfn "computeArrayNorm()                             = %f" <| computeArrayNorm()
    printfn "arrayNormWrapper([|1.0; 2.0; 3.0; 4.0 |]       = %f" <|  arrayNormWrapper [|1.0; 2.0; 3.0; 4.0 |]
    printfn "testVectorNorm()                               = %f" <| testVectorNorm()

    printfn "genArray 5                                     = %A" <| genArray 5
    printfn "genArray2 5                                    = %A" <| genArray2 5
    printfn "sumArray([1.0; 2.0; 3.0; 4.0], 4)                 = %A" <| vectorSum([| 1.0; 2.0; 3.0; 4.0 |], 4)
    0
