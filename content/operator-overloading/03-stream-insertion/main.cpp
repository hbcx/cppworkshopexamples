#include <iostream>
#include <ostream>

// Teach a type to print itself through std::ostream, so it works with std::cout,
// a file stream, a string stream -- anything that is an ostream.

class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}

    // Free function, declared friend so it can read x_ and y_ directly.
    // Left operand is the stream; right operand is the Point. Return the stream
    // by reference so that cout << p1 << p2 chains left to right.
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << '(' << p.x_ << ", " << p.y_ << ')';
        return os;
    }

private:
    int x_, y_;
};

int main() {
    Point a{1, 2};
    Point b{3, 4};

    // Chaining works because each << returns the same ostream&:
    std::cout << "a = " << a << ", b = " << b << "\n";   // a = (1, 2), b = (3, 4)
    return 0;
}
