#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Point {
    int x;
    int y;
};

// Equality is REQUIRED: after hashing to a bucket, an unordered container
// compares keys with == to tell a real match from a collision.
bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

// Specializing std::hash for YOUR OWN type inside namespace std is explicitly
// allowed. Combine the members' hashes (a simple mix here; the next example
// factors out a reusable hash_combine).
namespace std {
template <>
struct hash<Point> {
    size_t operator()(const Point& p) const {
        size_t hx = std::hash<int>{}(p.x);
        size_t hy = std::hash<int>{}(p.y);
        return hx ^ (hy + 0x9e3779b97f4a7c15ULL + (hx << 6) + (hx >> 2));
    }
};
}  // namespace std

int main() {
    std::cout << std::boolalpha;

    // With hash + ==, Point is a usable key.
    std::unordered_set<Point> seen{{1, 2}, {3, 4}};
    std::cout << "contains (1,2)? " << (seen.count({1, 2}) != 0) << "\n";   // true
    std::cout << "contains (2,1)? " << (seen.count({2, 1}) != 0) << "\n";   // false

    std::unordered_map<Point, std::string> labels;
    labels[{0, 0}] = "origin";
    labels[{1, 1}] = "diagonal";
    std::cout << "label (0,0) = " << labels[{0, 0}] << "\n";                // origin

    // NOTE: keys are const inside the container, so do not design a key whose
    // hashed fields you intend to change while it is stored.
    return 0;
}
