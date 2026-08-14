#include <utility>
#include <iostream>

namespace geo {
    struct Point {
        int x, y;
        int usedCustom;       // set by our swap, to prove which swap ran
        Point(int x_, int y_) : x(x_), y(y_), usedCustom(0) {}
    };
    // A custom swap for Point, found by ADL in namespace geo.
    void swap(Point& a, Point& b) noexcept {
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
        a.usedCustom = 1;
        b.usedCustom = 1;
    }
}

template <typename T>
void badSwap(T& a, T& b) {
    std::swap(a, b);          // BAD: fully qualified -- always the std one
}

template <typename T>
void goodSwap(T& a, T& b) {
    using std::swap;          // GOOD: bring std::swap in, then let ADL choose
    swap(a, b);               // finds geo::swap for a Point
}

int main() {
    std::cout << std::boolalpha;

    geo::Point p{1, 2}, q{3, 4};
    goodSwap(p, q);
    std::cout << "goodSwap used the custom swap? " << (p.usedCustom == 1) << '\n';

    geo::Point r{1, 2}, s{3, 4};
    badSwap(r, s);
    std::cout << "badSwap used the custom swap?  " << (r.usedCustom == 1) << '\n';
    return 0;
}
