#include <bit>
#include <iostream>
#include <string_view>
#include <type_traits>

enum class Feature : unsigned {
    Cache    = 1u << 0,
    Compress = 1u << 1,
    Encrypt  = 1u << 2,
    Verbose  = 1u << 3,
};
using U = std::underlying_type_t<Feature>;
constexpr Feature operator|(Feature a, Feature b) {
    return static_cast<Feature>(static_cast<U>(a) | static_cast<U>(b));
}

// A tiny table mapping each single-bit flag to its name, in bit order. Iterating
// the set bits and looking each one up is how a flags value is logged or
// serialized as readable names instead of a raw number.
struct Named { Feature flag; std::string_view name; };
constexpr Named kNames[] = {
    { Feature::Cache,    "Cache" },
    { Feature::Compress, "Compress" },
    { Feature::Encrypt,  "Encrypt" },
    { Feature::Verbose,  "Verbose" },
};

int main() {
    Feature f = Feature::Cache | Feature::Encrypt | Feature::Verbose;

    std::cout << "set flags:";
    for (U bits = static_cast<U>(f); bits != 0; bits &= bits - 1) {
        U low = bits & (~bits + 1);            // isolate the lowest set bit
        int index = std::countr_zero(bits);    // its position (C++20 <bit>)
        for (const Named& n : kNames) {         // look the single bit up by value
            if (static_cast<U>(n.flag) == low) {
                std::cout << ' ' << n.name << "(bit " << index << ')';
                break;
            }
        }
    }
    std::cout << '\n';

    std::cout << "popcount: " << std::popcount(static_cast<U>(f)) << '\n';   // 3
    return 0;
}
