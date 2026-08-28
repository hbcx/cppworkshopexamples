#include <iostream>
#include <ostream>

// Anti-pattern: operator<< written as a MEMBER. A member operator makes your type
// the left operand, but for stream insertion the left operand is the stream. The
// result compiles, yet it reads backwards and does not fit normal stream code.

class BadPoint {
public:
    BadPoint(int x, int y) : x_(x), y_(y) {}

    // BAD: member => the object is on the left, so you must write p << std::cout.
    // std::cout << p does NOT compile, and it cannot chain.
    void operator<<(std::ostream& os) const {
        os << '(' << x_ << ", " << y_ << ')';
    }

private:
    int x_, y_;
};

class GoodPoint {
public:
    GoodPoint(int x, int y) : x_(x), y_(y) {}

    // FIX: free friend, stream on the left, returns the stream so it chains.
    friend std::ostream& operator<<(std::ostream& os, const GoodPoint& p) {
        os << '(' << p.x_ << ", " << p.y_ << ')';
        return os;
    }

private:
    int x_, y_;
};

int main() {
    BadPoint bad{1, 2};
    // The only way to call the member version -- object first, stream second:
    bad << std::cout;
    std::cout << "  <- had to write 'bad << std::cout'\n";

    GoodPoint a{1, 2}, b{3, 4};
    // The free version reads the normal way and chains:
    std::cout << "points: " << a << " and " << b << "\n";   // points: (1, 2) and (3, 4)
    return 0;
}
