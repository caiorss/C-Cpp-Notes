#r "gsl.dll"



printfn "Gsl.Basic.hypot(80.0, 60.0) = %f " <| Gsl.Basic.hypot(80.0, 60.0)

let result = Gsl.Poly.polyEval [1.0; 2.0; 3.0 ; 4.0 ] 2.0
in printfn " Gsl.Poly.polyEval [1.0; 2.0; 3.0 ; 4.0 ] 2.0 = %f" <| result

let cpl = Gsl.gsl_complex_rect2(40.0, 40.0)
in printfn "Gsl.gsl_complex_rect(30., 20.) = %A" (Gsl.gslCplToTuple cpl)


