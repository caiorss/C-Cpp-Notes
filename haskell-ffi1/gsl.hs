-- | Load this file with: 
--  
--   $ stack exec -- ghci -lgsl
--

:set -XForeignFunctionInterface
:set -XOverloadedStrings

import Foreign.C.Types 
import Foreign.C.String (CString, newCString, castCharToCChar, castCCharToChar)
import Foreign.Storable
import Foreign.Ptr    
import Foreign.Marshal.Alloc 
import qualified Foreign.Marshal.Array as A
import System.IO.Unsafe (unsafePerformIO)
    
-- double gsl_poly_eval(const double c[], const int len, const double x);
foreign import ccall "gsl_poly_eval" gsl_polyEval :: Ptr Double -> Int -> Double -> IO Double 
                                                                       
:{
{- | Evaluate polynomial
     c[0] + c[1] x + c[2] x^2 + ... + c[len-1] x^(len-1)
-}   
polyEval :: [Double] -> Double -> Double 
polyEval poly x = unsafePerformIO $ do
  arr <- A.newArray poly
  gsl_polyEval arr (length poly) x       
:}   


