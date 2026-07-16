#include <array>
#include <iostream>
#include <stdexcept>

// A C function that wants a pointer and a count -- the shape std::array feeds
// without any ceremony.
static int sum_c_style(const int* values, std::size_t count) {
    int total = 0;
    for (std::size_t i = 0; i < count; ++i) {
        total += values[i];
    }
    return total;
}

int main() {
    std::array<int, 5> temps{18, 21, 19, 23, 20};   // elements live inside the
                                                    // object: no allocation
    std::cout << "size  = " << temps.size() << "   (fixed at compile time)\n";
    std::cout << "front = " << temps.front() << ", back = " << temps.back() << '\n';

    // The container interface a C array does not have.
    std::cout << "values:";
    for (int t : temps) {                  // range-for works: begin()/end() exist
        std::cout << ' ' << t;
    }
    std::cout << '\n';

    // Two accessors, two contracts. operator[] does not check:
    temps[0] = 17;
    // at() does, and throws instead of corrupting memory:
    try {
        temps.at(9) = 0;
    } catch (const std::out_of_range&) {
        std::cout << "at(9) threw out_of_range  (temps[9] would just be UB)\n";
    }

    // No push_back, no resize, no clear -- the size is part of the type. What
    // you can do is overwrite every element at once.
    std::array<int, 5> zeros;
    zeros.fill(0);
    std::cout << "after fill(0): " << zeros[0] << ' ' << zeros[4] << '\n';

    // data() is the bridge to C APIs.
    std::cout << "sum via data(): " << sum_c_style(temps.data(), temps.size()) << '\n';
    return 0;
}
