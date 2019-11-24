/** 
 *   Author: Caio Rodrigues  - caiorss [dot] rodrigues [at] gmail [dot] com 
 *   Brief:  Numeric range iterator implementation 
 *  
 *------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric> // std::accumulate
#include <algorithm>

template<typename T>
class RangeIterator
{
public:

    // Type that iterator refers to
    using value_type = T;
    // Pointer
    using pointer = T*;
    // Reference
    using reference = T*;

    // Tag Indicates the iterator's capabilities
    using iterator_category = std::input_iterator_tag;

    // Type of difference between two iterators.
    using difference_type = int;

    RangeIterator(T value)
        : m_step(0), m_value(value)
    { }

    RangeIterator(T value, T step)
        : m_step(step), m_value(value)
    { }

    // Dereference operator:
    value_type& operator*() {
        return m_value;
    }

    // Dereference operator:
    const value_type&  operator*() const  {
        return m_value;
    }

    value_type* operator->() {
        return &m_value;
    }

    // Prefix increment operator
    RangeIterator& operator++() {
        m_value += m_step;
        return *this;
    }

    // Postfix increment operator
    RangeIterator& operator++(int) {
        m_value += m_step;
        return *this;
    }

    bool operator==(const RangeIterator& rhs) const
    {
        return this->m_value > rhs.m_value;
    }

    bool operator!=(const RangeIterator& rhs) const
    {
        return !this->operator==(rhs);
    }

private:
    T const m_step;
    T  m_value;
};


template<typename T>
class NumericRange {
public:

    NumericRange(T start, T step, T stop):
        m_start(start), m_step(step), m_stop(stop) { }

    RangeIterator<T> begin()
    {
        return RangeIterator<T>(m_start, m_step);
    }

    RangeIterator<T> end()
    {
        return RangeIterator<T>(m_stop);
    }

private:
    T m_start;
    T m_step;
    T m_stop;

};


int main(int argc, char** argv)
{

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "\n ==== EXPERIMENT 1 ==============================" << std::endl;

    {
        auto range = NumericRange(-10.0, 2.5, 10.0);
        int n = 0;

        for(auto it = range.begin(); it != range.end(); it++ )
        {
            auto label =  std::string(" x[") + std::to_string(n++) + "] = ";
            std::cout << std::setw(10) << label
                      << std::setw(6) << *it
                      << std::endl;
        }

    }


    std::cout << "\n ==== EXPERIMENT 2 ==============================" << std::endl;
    {
        int n = 0;

        for(auto x: NumericRange(-10.0, 2.5, 10.0))
        {
            auto label =  std::string(" x[") + std::to_string(n++) + "] = ";
            std::cout << std::setw(10) << label
                      << std::setw(6) << x
                      << std::endl;
        }

    }

    std::cout << "\n ==== EXPERIMENT 3 - STL algorithms ============" << std::endl;
    {
        auto range = NumericRange(-10.0, 2.5, 10.0);

        double result = std::accumulate( range.begin(), range.end()
                                        , 0.0 // Initial value of accumulator
                                        , [](double acc, double x)
                                        {
                                            return acc + x;
                                        });

        // Initialize container from iterators
        std::vector<double> xs(range.begin(), range.end());


        std::cout << " [INFO] Result = " << result << std::endl;
    }

	return 0;
}
