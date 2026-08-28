#include <iostream>
#include <compare>

// A version number ordered major, then minor, then patch. Before C++20 this meant
// writing (or deriving) six operators. Now it is one defaulted line.

struct Version {
    int major;
    int minor;
    int patch;

    // Generates <, >, <=, >= from the memberwise comparison, and also causes ==
    // (and !=) to be defaulted. Members are compared in declaration order.
    auto operator<=>(const Version&) const = default;
};

int main() {
    Version v1{1, 4, 0};
    Version v2{1, 10, 0};
    Version v3{1, 4, 0};

    // Equality came along for free with the defaulted <=>:
    std::cout << "v1 == v3? " << (v1 == v3) << "\n";   // 1
    std::cout << "v1 != v2? " << (v1 != v2) << "\n";   // 1

    // Ordering: minor 4 comes before 10 because major ties and minor decides.
    std::cout << "v1 <  v2? " << (v1 < v2)  << "\n";   // 1
    std::cout << "v2 >  v1? " << (v2 > v1)  << "\n";   // 1
    std::cout << "v1 <= v3? " << (v1 <= v3) << "\n";   // 1
    std::cout << "v2 >= v1? " << (v2 >= v1) << "\n";   // 1
    return 0;
}
