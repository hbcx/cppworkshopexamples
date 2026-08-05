// Anti-pattern: pimpl on a small hot value type.
//
// A Point is two doubles. Wrapping it in pimpl adds a heap allocation per
// object, an indirection on every access, and blocks trivial copy / constexpr /
// inlining -- all to hide internals that ARE the type's meaning and never
// change. There is no firewall to buy here, only cost.
//
// The fix: a plain struct. Use pimpl only when a firewall or ABI actually
// matters, not for small value types.

#include <iostream>
#include <memory>

// ---------- pimpl where it earns nothing ----------
namespace bad {

class Point {
public:
    Point(double x, double y) : impl_(new Impl{x, y}) {}          // heap-allocates
    Point(const Point& other) : impl_(new Impl(*other.impl_)) {}  // copy allocates again
    double x() const { return impl_->x; }
    double y() const { return impl_->y; }

private:
    struct Impl {
        double x;
        double y;
    };
    std::unique_ptr<Impl> impl_;
};

} // namespace bad

// ---------- a plain value type ----------
namespace good {

struct Point {
    double x;
    double y;
};

} // namespace good

int main() {
    bad::Point b(1.0, 2.0);
    bad::Point b2 = b; // a heap allocation just to copy two doubles
    std::cout << "[bad]  copied via heap alloc: (" << b2.x() << "," << b2.y() << ")\n";

    good::Point g{1.0, 2.0};
    good::Point g2 = g; // trivial copy, no allocation
    std::cout << "[good] copied trivially: (" << g2.x << "," << g2.y << ")\n";
}
