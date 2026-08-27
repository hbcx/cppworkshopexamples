#include <iostream>
#include <cstddef>
#include <tuple>       // std::tuple_size / std::tuple_element / integral_constant

// A type does not have to be a pair, tuple, array or aggregate to support
// structured bindings. Implement the "tuple protocol" and your own class becomes
// bindable: specialize std::tuple_size and std::tuple_element, and provide a
// get<I>. Here a fixed 2D vector exposes its components by index.

class Vec2 {
public:
    Vec2(double x, double y) : x_(x), y_(y) {}
    double x() const { return x_; }
    double y() const { return y_; }
private:
    double x_, y_;
};

// get<I> returns the Ith component. A free function in the same namespace is
// found by argument-dependent lookup, which is what the binding uses.
template <std::size_t I>
double get(const Vec2& v) {
    static_assert(I < 2, "Vec2 has two components");
    if constexpr (I == 0) return v.x();
    else                  return v.y();
}

// The protocol lives in namespace std: how many elements, and each element type.
namespace std {
    template <>
    struct tuple_size<Vec2> : std::integral_constant<std::size_t, 2> {};

    template <std::size_t I>
    struct tuple_element<I, Vec2> { using type = double; };
}

int main() {
    Vec2 v(1.5, -2.5);

    // A structured binding now works on Vec2, calling get<0> and get<1>.
    auto [x, y] = v;
    std::cout << "Vec2 unpacked: x=" << x << " y=" << y << "\n";

    // This is exactly how std::tuple, std::pair and std::array become bindable.
    return 0;
}
