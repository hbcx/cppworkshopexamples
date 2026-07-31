#include <iostream>

// Anti-pattern: a full Builder for a value with two required fields and nothing
// else -- no optional fields, no defaults, no validation. All ceremony.
class PointOverbuilt {
public:
    class Builder;
    int x() const { return x_; }
    int y() const { return y_; }
private:
    PointOverbuilt(int x, int y) : x_(x), y_(y) {}
    friend class Builder;
    int x_ = 0;
    int y_ = 0;
};
class PointOverbuilt::Builder {
public:
    Builder& x(int v) { x_ = v; return *this; }
    Builder& y(int v) { y_ = v; return *this; }
    PointOverbuilt build() const { return PointOverbuilt(x_, y_); }
private:
    int x_ = 0;   // and a caller who forgets .y() silently gets 0
    int y_ = 0;
};

// Fix: it is an aggregate. Both fields are required at the call site, there is
// no build() to remember, and nothing to maintain.
struct Point {
    int x;
    int y;
};

int main() {
    // Overbuilt: a nested class and a terminal call to say "3, 4".
    PointOverbuilt a = PointOverbuilt::Builder().x(3).y(4).build();
    std::cout << "overbuilt: " << a.x() << ", " << a.y() << '\n';

    // The builder's own trap: forget a setter and you get a silent default.
    PointOverbuilt oops = PointOverbuilt::Builder().x(3).build();   // y defaulted to 0
    std::cout << "forgot y:  " << oops.x() << ", " << oops.y() << '\n';

    // Fix: brace-init makes both coordinates required and needs no machinery.
    Point p{3, 4};
    std::cout << "aggregate: " << p.x << ", " << p.y << '\n';
    return 0;
}
