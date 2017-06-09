///
/// Linux LibC FFI wrappers examples.
///
///
open System 
open System.Runtime.InteropServices


/// Linux LibC FFI
///


//  int puts(const char *s);
[<DllImport("libc.so.6", EntryPoint="puts")>]
extern void puts(string)

// int gethostname(char *name, size_t len);
[<DllImport("libc.so.6", EntryPoint="gethostname")>]
extern int private c_getHostname(System.Text.StringBuilder, int len)

// double cbrt(double x);
[<DllImport("libm.so.6", EntryPoint="cbrt")>]
extern double cbrt(double)

/// #include<math.h>
///
/// On success, these functions return the length of a right-angled
/// triangle with sides of length x and y
///       
/// double hypot(double x, double y);
[<DllImport("libm.so.6", EntryPoint="hypot")>]
extern double hypot(double, double)

    
(**
Header: time.h
   
    struct tm {
        int tm_sec;    /* Seconds (0-60) */
        int tm_min;    /* Minutes (0-59) */
        int tm_hour;   /* Hours (0-23) */
        int tm_mday;   /* Day of the month (1-31) */
        int tm_mon;    /* Month (0-11) */
        int tm_year;   /* Year - 1900 */
        int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
        int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
        int tm_isdst;  /* Daylight saving time */
    };


*)
[<Struct>]
[<StructLayout(LayoutKind.Sequential)>]
type UnixTm =
    struct
        val tm_sec:   int;
        val tm_min:   int;
        val tm_hour:  int;
        val tm_mday:  int;
        val tm_mon:   int;
        val tm_year:  int;
        val tm_wday:  int;
        val tm_yday:  int;
        val tm_isdst: int
        val tm_gmtoff: int;
        
        [<MarshalAs(UnmanagedType.LPWStr)>]
        val tm_zone:  string;
        //val tm_zone: IntPtr;
    end


/// Returns an Unix timestamp
///
/// Note: time_t is an 32 bits integer on 32 bits machines
///       and 64 bits on a 64-bits machine or OS.
//
/// time_t time(time_t *t) 
/// time_t time(NULL)
///
///    
[<DllImport("libc.so.6", EntryPoint="time")>]
extern System.Int64 ctime()

// Note: When the *timep pointer is null. It returns
// the current time.
//
// struct tm *localtime(const time_t *timep);
//
[<DllImport("libc.so.6", EntryPoint="localtime")>]
extern UnixTm c_localtime(System.Int64&)

[<DllImport("libc.so.6", EntryPoint="localtime")>]
extern IntPtr c_localtime2(System.Int64&)


let localtime time =
    let mutable tref = time 
    c_localtime(&tref)

let localtime2(): UnixTm =
    let mutable tref = 0L
    let ptr = c_localtime2(&tref)
    Marshal.PtrToStructure(ptr)

let getHostName () =
    let sb = new System.Text.StringBuilder(64)
    ignore <| c_getHostname(sb, 64)
    sb.ToString()    

let testLibc() =
    puts "\n\nHello world!! It works!!"
    printfn "getHostName() = %s" <| getHostName()
    printfn "Array.map Libc.cbrt  [| 1.0 ; 8.0; 27.0; 64 |] = %A" <| Array.map cbrt [| 1.0 ; 8.0; 27.0; 64.0|]


let showTm(tm: UnixTm) =
    printfn "tm.tm_year = %d" tm.tm_year
    printfn "tm.tm_day  = %d" tm.tm_mday

let testTime() =
    let size = Marshal.SizeOf(typeof<UnixTm>)
    printfn "sizeof(struct tm) = %d" size
    showTm <| localtime 1496843850L

testTime() 

