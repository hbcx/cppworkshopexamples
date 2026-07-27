// To use your own type as a key, a hash container needs two things: a way to
// compare keys for equality, and a way to hash them. Built-in and standard-
// library types come with a std::hash specialization; a user type has none, so
// you supply a hash functor (here PointHash) and an operator==. The hash functor
// is passed as the third template argument.

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace {

struct Point {
    int x;
    int y;
};

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

struct PointHash {
    std::size_t operator()(const Point& p) const {
        // Combine BOTH members. Hashing only p.x would put every point in a
        // column into one bucket -- correct, but slow (see the weak-hash
        // anti-pattern). This mix spreads distinct points across buckets.
        const std::size_t hx = std::hash<int>()(p.x);
        const std::size_t hy = std::hash<int>()(p.y);
        return hx ^ (hy * 0x9e3779b9u + (hx << 6) + (hx >> 2));
    }
};

}  // namespace

int main() {
    std::unordered_map<Point, std::string, PointHash> labels;
    labels[Point{0, 0}] = "origin";
    labels[Point{1, 0}] = "east";
    labels[Point{0, 1}] = "north";

    std::cout << "(0,0) -> " << labels.at(Point{0, 0}) << "\n";
    std::cout << "(1,0) -> " << labels.at(Point{1, 0}) << "\n";
    std::cout << "(0,1) -> " << labels.at(Point{0, 1}) << "\n";
    std::cout << "has (5,5)? " << (labels.count(Point{5, 5}) ? "yes" : "no") << "\n";
    std::cout << "size = " << labels.size() << "\n";
    return 0;
}
