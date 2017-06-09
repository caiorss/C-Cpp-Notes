:set -XForeignFunctionInterface
:set -XOverloadedStrings

import Foreign.C.Types 
import Foreign.C.String (CString, newCString, castCharToCChar, castCCharToChar)
import Foreign.Storable
import Foreign.Ptr    
import Foreign.Marshal.Alloc 
import qualified Foreign.Marshal.Array as A
    
---- ============ Pure Functions ==================== -----

-- double cbrt(double x);
foreign import ccall "cbrt" cbrt :: Double -> Double

--  double hypot(double x, double y);                                    
foreign import ccall "hypot" hypot :: Double -> Double -> Double
   

---- ============ Functions with Side Effects ======== ---- 
    
-- int puts(const char *s);    
foreign import ccall "puts" cputs :: CString -> IO ()
puts s = newCString s >>= cputs

-- pid_t getpid(void);
foreign import ccall "getpid" getpid :: IO Int                                    

-- Execute Shell command and return after the command has been completed.
--
--  int system(const char *command);
foreign import ccall "system" csystem :: CString -> IO Int
system cmd = newCString cmd >>= csystem

-- int gethostname(char *name, size_t len);
--
foreign import ccall "gethostname" c_gethostname :: (Ptr CChar) -> Int -> IO Int             

:{
gethostname :: IO String
gethostname = do
  -- Allocate 255 bytes (C- char Characters)
  ptrChar <- A.mallocArray 255             
  c_gethostname ptrChar 255
  -- Get all array CChar array elements until the null character \0 
  name    <- A.peekArray0 (castCharToCChar '\0') ptrChar
  -- Convert [CChar] to [Char] or String            
  return $ map castCCharToChar name
:}
   
:{
testLibc :: IO ()
testLibc = do
  hname <- gethostname
  putStrLn "\n\n\nLibC FFI Tests\n---------------------"
  puts "Hello world São Paulo city"
  putStrLn $ "Hostname = " ++ hname 
  system "uname -a"
  system "wrong"       
  putStrLn $ "cbrt 27.0       = " ++ show (cbrt 27.0)       
  putStrLn $ "hypot 80.0 60.0 = " ++ show (hypot 80.0 60.0)     

:}   

testLibc 
