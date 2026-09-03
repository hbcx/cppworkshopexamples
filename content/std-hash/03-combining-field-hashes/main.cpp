#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

// A reusable hash_combine -- the well-known mixing step (as popularised by Boost).
// It folds each field's hash into a running seed with a large odd constant and
// shifts, so the result depends on the VALUE and ORDER of every field.
template <typename T>
void hash_combine(std::size_t& seed, const T& value) {
    seed ^= std::hash<T>{}(value) + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

struct Person {
    std::string name;
    int age;
};

bool operator==(const Person& a, const Person& b) {
    return a.name == b.name && a.age == b.age;
}

namespace std {
template <>
struct hash<Person> {
    size_t operator()(const Person& p) const {
        size_t seed = 0;
        hash_combine(seed, p.name);
        hash_combine(seed, p.age);
        return seed;
    }
};
}  // namespace std

int main() {
    std::cout << std::boolalpha;

    // A naive XOR of field hashes is SYMMETRIC: swapping the fields gives the same
    // result, so (1,2) and (2,1) collide -- bad for a coordinate-like key.
    auto naive = [](int a, int b){ return std::hash<int>{}(a) ^ std::hash<int>{}(b); };
    std::cout << "naive(1,2) == naive(2,1)?       " << (naive(1, 2) == naive(2, 1)) << "\n";  // true

    // hash_combine is order-sensitive, so the same pair does not collide.
    auto combined = [](int a, int b){
        std::size_t s = 0; hash_combine(s, a); hash_combine(s, b); return s;
    };
    std::cout << "combined(1,2) == combined(2,1)? " << (combined(1, 2) == combined(2, 1)) << "\n";  // false

    // And the real multi-field key built from it works as expected:
    std::unordered_set<Person> people{{"Ada", 36}, {"Ben", 36}};
    std::cout << "has Ada/36? " << (people.count({"Ada", 36}) != 0) << "\n";   // true
    std::cout << "has Ada/40? " << (people.count({"Ada", 40}) != 0) << "\n";   // false
    return 0;
}
