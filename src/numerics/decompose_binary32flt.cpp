/**
 *   File:   decompose_binary32flt.cpp
 *   Author: Caio Rodrigues Soares Silva
 *   Brief:  Sample C++ program to decompose IEE754 32 bits floating point.
 *
 *   Note:   The results of this app can be checked with the online calculator:
 *           https://calculla.com/floating_point_numbers
 **************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cstdint>
#include <map>

#define SHOW_FLOAT(expr) show_float(#expr, (expr))

template<typename T>
std::string num2hexstr(T number, int padding = 8)
{
    std::stringstream ss;
    if(padding != 0){
        ss << "0x" << std::hex
           <<  std::setfill('0') << std::setw(padding) << number;
    } else {
        ss << "0x" << std::hex  << number;
    }
    return ss.str();
}

std::string to_hexfloat(float number)
{
    std::stringstream ss;
    ss << std::hexfloat << number;
    return ss.str();
}

// Decompose float point
void show_float(const char* expr, float x)
{
    constexpr int w1 = 34;
    constexpr int w2 = 4;

    // Float point classification constants
    static auto const fpmap = std::map<int, const char*>{
         {FP_INFINITE,  "FP_INFINITE"}
        ,{FP_NAN,       "FP_NAN"}
        ,{FP_NORMAL,    "PF_NORMAL"}
        ,{FP_SUBNORMAL, "FP_SUBNORMAL"}
        ,{FP_ZERO,      "FP_ZERO"}
    };

    // Integer represetation extracted
    // through type punning aka memory reinterpretation
    std::uint32_t* pn       = reinterpret_cast<std::uint32_t*>(&x);
    std::uint32_t  value    = *pn;
    std::uint32_t  mantissa = value & ((1 << 23) - 1);
    std::uint32_t  raw_exp  = (value >> 23) & 0xFF;
    std::uint32_t  sign     = (value >> 31) & 0x01;

    auto print_row = [&w1, &w2](const char* label, auto const& value)
    {
        std::cout << std::setw(w1) << std::right << label
                  << std::setw(w2) << std::left  << value
                  << "\n";
    };

    print_row("Input Expression: ", expr);
    print_row("Classification: ", fpmap.at(std::fpclassify(x)));
    print_row("Decimal Representation: ", x);
    print_row("Hexadecimal Representation: ", to_hexfloat(x));
    print_row("Integer Representation (hex): ", num2hexstr(value));
    print_row("Sign bit: ", sign);
    print_row("Exponent: ", static_cast<std::int32_t>(raw_exp) - 127);
    print_row("Raw Exponent: ", raw_exp);
    print_row("Raw Exponent (hex): ", num2hexstr(raw_exp, 0));
    print_row("Mantissa (hex): ", num2hexstr(mantissa, 0));    
    std::cout << "\n\n";
}


int main()
{
    std::puts("\n==== Decomposition of Single Precision Float Points =====\n");

    // Signed Zero
    SHOW_FLOAT(+0.0f);
    SHOW_FLOAT(-0.0f);

    // NaN - Not a number
    SHOW_FLOAT(+std::numeric_limits<float>::quiet_NaN());
    SHOW_FLOAT(-std::numeric_limits<float>::quiet_NaN());

    // Positive and negative Infinity
    SHOW_FLOAT(+std::numeric_limits<float>::infinity());
    SHOW_FLOAT(-std::numeric_limits<float>::infinity());

    // Subnormal
    SHOW_FLOAT(+std::numeric_limits<float>::min());
    SHOW_FLOAT(+std::numeric_limits<float>::min() / 100.0f);
    SHOW_FLOAT(-std::numeric_limits<float>::min() / 1000000.0f);

    // Epsilon
    SHOW_FLOAT(+std::numeric_limits<float>::epsilon());

    // Normal Numbers
    SHOW_FLOAT(1.0f);
    SHOW_FLOAT(0.5f);
    SHOW_FLOAT(1E-5f);
    SHOW_FLOAT(1.051646E4f);
    SHOW_FLOAT(-99000134.3401f);
    SHOW_FLOAT(std::numeric_limits<float>::max());
    SHOW_FLOAT(std::numeric_limits<float>::lowest());

    return 0;
}
