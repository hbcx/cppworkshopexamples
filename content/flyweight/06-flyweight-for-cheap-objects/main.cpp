#include <iostream>
#include <map>
#include <memory>
#include <utility>

// A tiny, trivially-copyable value.
struct Point {
    int x;
    int y;
};

// ANTI-PATTERN: interning a tiny value with a Flyweight factory. Flyweight pays
// off when many objects share a LARGE intrinsic state; for an 8-byte value it is
// pure overhead -- the shared_ptr handle alone is as big as the value, and each
// distinct value also costs a heap control block and a map node, plus a lookup on
// every access. You spend more memory and time than simply copying would.
class PointFactory {
public:
    std::shared_ptr<Point> get(int x, int y) {
        auto& p = pool_[std::make_pair(x, y)];
        if (!p) p = std::make_shared<Point>(Point{x, y});
        return p;
    }
private:
    std::map<std::pair<int, int>, std::shared_ptr<Point>> pool_;
};

int main() {
    std::cout << "== flyweight for a tiny value ==\n";
    PointFactory factory;
    auto p = factory.get(1, 2);
    std::cout << "sizeof(Point)              = " << sizeof(Point) << " bytes\n";
    std::cout << "sizeof(shared_ptr handle)  = " << sizeof(p) << " bytes"
              << " (the handle alone is at least as big as the value)\n";
    std::cout << "...plus a heap control block and a map node per distinct value\n";

    std::cout << "== fix: store the value directly ==\n";
    Point q{1, 2};                     // no heap, no lookup, no sharing needed
    std::cout << "Point stored inline        = " << sizeof(q) << " bytes\n";
    std::cout << "point is (" << q.x << "," << q.y << ")\n";
    return 0;
}
