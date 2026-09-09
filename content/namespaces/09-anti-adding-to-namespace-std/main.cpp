#include <cstddef>
#include <functional>
#include <iostream>
#include <unordered_set>

struct Point {
    int x;
    int y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

// ALLOWED: add a template SPECIALIZATION of a std class template for your OWN
// type. The standard explicitly permits this for std::hash, std::less, formatter,
// and a few others -- it is how a user type becomes usable as a hash-map key.
template <>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
};

// ANTI-PATTERN (described, NOT compiled -- it is undefined behaviour):
//
//   namespace std {
//       // adding a NEW function, type, or overload to namespace std is UB,
//       // even if it compiles and links:
//       void describe(const Point&) { ... }
//       template <class T> void my_algorithm(T&) { ... }
//   }
//
// You may add specializations of permitted std templates for your own types.
// You may NOT add new declarations to namespace std. Put your own free functions
// in your own namespace and let ADL find them.

int main() {
    std::unordered_set<Point> seen;   // relies on the std::hash<Point> specialization
    seen.insert({1, 2});
    seen.insert({1, 2});              // duplicate, folded away
    seen.insert({3, 4});
    std::cout << "distinct points = " << seen.size() << "\n";
    return 0;
}
