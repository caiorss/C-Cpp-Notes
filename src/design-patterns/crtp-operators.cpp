/*  Author: Caio Rodrigues - caiorss [dot] rodrigues [at] gmail [dot] com
 *  Brief:  Using CRTP - simplifying for operator overloading
 *  URL:    https://caiorss.github.io/C-Cpp-Notes/cpp-design-patterns.html
 *********************************************************************/

#include <iostream>


/* CRTP - Curious Recurring Template
 *
 *  Generates the operators (==) and (!=) for a class that implements
 * the member function:
 *
 *    bool T::equal(T const& rhs) const;
 *
 *------------------------------------------------------------*/
template<typename Impl>
class EqualityOperator
{
public:

    friend bool operator==(Impl const& lhs, Impl const& rhs)
    {
        return lhs.equal(rhs);
    }

    friend bool operator!=(Impl const& lhs, Impl const& rhs)
    {
        return lhs.equal(rhs);
    }

};

/* Implements operator (+) for Impl + T.
 * The class Impl must implement the member function
 *
 *    Impl Impl::add(T const& T) const
 *
 *-------------------------------------------------*/
template<typename Impl, typename T>
struct AddOperator
{
public:

    friend Impl operator+(Impl const& lhs, T const& rhs)
    {
        return lhs.add(rhs);
    }

    friend Impl operator+(T const& lhs, Impl const& rhs)
    {
        return rhs.add(lhs);
    }

};

template<typename Impl>
struct StreamInsertionOperator
{
public:

    friend std::ostream& operator<<(std::ostream& os, Impl const& rhs)
    {
        rhs.printme(os);
        return os;
    }

};



class Point2D: public EqualityOperator<Point2D>
             , public AddOperator<Point2D, int>
             , public StreamInsertionOperator<Point2D>
{
public:
    int x;
    int y;
    Point2D(int x, int y): x(x), y(y) { }

    // Required by:
    void printme(std::ostream& os) const
    {
        os << "Point2D[ x = " << x << " ; y = " << y << " ] " << std::endl;
    }

private:
    // Allows class EqualityOperator<Point2D> to access private
    // members of this class.
    friend class EqualityOperator<Point2D>;
    friend struct AddOperator<Point2D, int>;

    // Required by: EqualityOperator<Point2D>
    bool equal(Point2D const& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    // Required by: AddOperator<Point2D, int>
    Point2D add(int rhs) const
    {
        return Point2D(this->x + rhs, this->y + rhs);
    }

};


int main(int argc, char** argv)
{
    Point2D p1(10, 20);
    Point2D p2(25, 30);
    Point2D p3(10, 20);

    std::cout << std::boolalpha;

    std::cout << " p1 === p2 ? => " << (p1 == p2) << std::endl;
    std::cout << " p1 === p3 ? => " << (p1 == p3) << std::endl;

    std::cout <<" p1 + 10 = " << p1 + 10 << std::endl;
    std::cout <<" 10 + p1 = " << p1 + 10 << std::endl;

	return 0;
}
