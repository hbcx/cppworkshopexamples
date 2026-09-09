#include <iostream>
#include <utility>

namespace math {
    struct Vec {
        int x;
        int y;
    };
    // operator+ and operator<< live in the SAME namespace as Vec. An unqualified
    // use finds them by ADL, because an argument's type (Vec) lives in math.
    Vec operator+(const Vec& a, const Vec& b) { return {a.x + b.x, a.y + b.y}; }
    std::ostream& operator<<(std::ostream& os, const Vec& v) {
        return os << "(" << v.x << "," << v.y << ")";
    }
}

namespace lib {
    struct Widget {
        int id;
    };
    // A better swap for Widget, also found by ADL.
    void swap(Widget& a, Widget& b) noexcept {
        std::swap(a.id, b.id);
        std::cout << "  lib::swap called\n";
    }
}

// Generic code should NOT hard-qualify swap: the two-step lets ADL pick a type's
// own swap when it has one, and fall back to std::swap otherwise.
template <typename T>
void generic_swap(T& a, T& b) {
    using std::swap;
    swap(a, b);
}

int main() {
    math::Vec a{1, 2}, b{3, 4};
    std::cout << "a + b = " << (a + b) << "\n";   // ADL finds math::operator+ and math::operator<<

    lib::Widget w1{1}, w2{2};
    generic_swap(w1, w2);                          // ADL finds lib::swap
    std::cout << "  widgets now " << w1.id << " " << w2.id << "\n";

    int x = 10, y = 20;
    generic_swap(x, y);                            // no lib::swap for int -> std::swap
    std::cout << "  ints now " << x << " " << y << "\n";
    return 0;
}
