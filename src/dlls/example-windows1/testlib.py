#===========================================================================
#          File:  testlib.py
#          Brief: Python wrapper for the DLL - Shared Library testlib.dll 
# Python Version: 3.0
#         Author: Caio Rodrigues
#=========================================================================
import ctypes

# Library handler 
_lib = ctypes.cdll.LoadLibrary("testlib.dll")

# Startup ctypes FFI - Foreign Function Interface 
def _config():
    print("Intializing library")
    # ======= std::vector<double> and Linalg:: namespace ==========##
    
    # hVectorD testlib_vectorD_make0(size_t n, double x)
    _lib.testlib_vectorD_make0.argtypes = [ctypes.c_int, ctypes.c_double]
    _lib.testlib_vectorD_make0.restype  = ctypes.c_void_p

    # hVectorD testlib_vectorD_make1(size_t n, double array [])
    _lib.testlib_vectorD_make1.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_double)]
    _lib.testlib_vectorD_make1.restype  = ctypes.c_void_p    

    # void testlib_vectorD_Linalg_printVector(const char* name, hVectorD hv)
    _lib.testlib_vectorD_Linalg_printVector.argtypes = [ctypes.POINTER(ctypes.c_char), ctypes.c_void_p ]
    _lib.testlib_vectorD_Linalg_printVector.restype = None

    # Set vector elements hv[n] = x
    # void testlib_vectorD_set(hVectorD hv, size_t n, double x)
    _lib.testlib_vectorD_set.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_double]
    _lib.testlib_vectorD_set.restype  = None

    # double Linalg::norm(const std::vector<double>& xs)
    # double testlib_vectorD_Linalg_norm(hVectorD hv)
    _lib.testlib_vectorD_Linalg_norm.argtypes = [ ctypes.c_void_p ]
    _lib.testlib_vectorD_Linalg_norm.restype  = ctypes.c_double

    # Function: void testlib_vectorD_delete(hVectorD hv)
    _lib.testlib_vectorD_delete.argtypes = [ ctypes.c_void_p ]
    _lib.testlib_vectorD_delete.restype = None

    #======== C-wrappers for function interface class InterfaceClass =============#

    # InterfaceClass* teslib_InterfaceClass_factory(const char* class_id)
    _lib.teslib_InterfaceClass_factory.argtypes = [ ctypes.POINTER(ctypes.c_char) ]
    _lib.teslib_InterfaceClass_factory.restype  = ctypes.c_void_p

    # const char* testlib_InterfaceClass_getID(InterfaceClass* hinst)
    _lib.testlib_InterfaceClass_getID.argtypes = [ ctypes.c_void_p ]
    _lib.testlib_InterfaceClass_getID.restype  = ctypes.POINTER(ctypes.c_char)
    
    # const char* testlib_InterfaceClass_getName(InterfaceClass* hinst)   
    _lib.testlib_InterfaceClass_getName.argtypes = [ ctypes.c_void_p ]
    _lib.testlib_InterfaceClass_getName.restype  = ctypes.POINTER(ctypes.c_char)

    # void testlib_InterfaceClass_setName(InterfaceClass* hinst, const char* name)
    _lib.testlib_InterfaceClass_setName.argtypes = [ ctypes.c_void_p, ctypes.POINTER(ctypes.c_char) ]
    _lib.testlib_InterfaceClass_setName.restype = None
    
    print("Library initialized OK.")

# Initializae module.
_config()

class VectorD:
    def __init__(self, handle):
        self.hnd = ctypes.c_void_p(handle)
        self.name = "std::vector<double> vx"

    @classmethod
    def fromValue(cls, size, x):
        return VectorD(_lib.testlib_vectorD_make0(size, x))

    @classmethod
    def fromArray(cls, array):
        carray_size_n = ctypes.c_double * len(array)
        return VectorD(_lib.testlib_vectorD_make1(len(array), carray_size_n(*array)))
        
    # Destructor 
    def __del__(self):
        print(" [TRACE] - Vector disposed - C++ Destructor invoked Ok.")
        _lib.testlib_vectorD_delete(self.hnd)

    def setName(self, name):
        self.name = name
        
    # Display vector 
    def disp(self):
        _lib.testlib_vectorD_Linalg_printVector(self.name.encode('utf-8'), self.hnd)

    # Set element at nth position 
    def set(self, idx, x):
        _lib.testlib_vectorD_set(self.hnd, idx, x)

    def norm(self):
        return _lib.testlib_vectorD_Linalg_norm(self.hnd)


# Proxy for C++ Interface class in the shared library 
class CPPInterfaceClass:
    # Constructor 
    def __init__(self, handle):
        self.hnd = ctypes.c_void_p(handle)

    # Destructor 
    def __del__(self):
        # Call C++ destructor
        # print(" [__del__] => self.hnd = " + str(self.hnd))
        _lib.testlib_InterfaceClass_delete(self.hnd)

    @classmethod
    def factory(cls, classID):
        return CPPInterfaceClass(_lib.teslib_InterfaceClass_factory(classID.encode('utf-8')))
        
    @classmethod
    def makeA(cls):              
        "Instantiate the class ImplementationA from the DLL."
        return CPPInterfaceClass.factory("ImplementationA")

    @classmethod
    def makeB(cls):              
        "Instantiate the class ImplementationB from the DLL."
        return CPPInterfaceClass.factory("ImplementationB")
    
    def getType(self):
        return ctypes.string_at(_lib.testlib_InterfaceClass_getID(self.hnd)).decode('utf-8')

    def getName(self):
        return ctypes.string_at(_lib.testlib_InterfaceClass_getName(self.hnd)).decode('utf-8')

    def setName(self, name):
        _lib.testlib_InterfaceClass_setName(self.hnd, name.encode('utf-8'))

    # String representation 
    def __str__(self):
        s   = "CInterfaceClass ; type = " + self.getType()
        s +=  " - name =  " + self.getName() + "\n"
        return s

    # Make class printable in the REPL
    def __repr__(self):
        return self.__str__()

