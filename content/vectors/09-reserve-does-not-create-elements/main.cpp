#include <cstddef>
#include <iostream>
#include <vector>

// Anti-pattern: reserve(n) only sets capacity -- it does NOT create elements.
// size() stays 0, so v[i] = ... writes to slots that do not exist yet (undefined
// behaviour), and every size()-based loop or range-for sees nothing.
static void print(const std::vector<int>& v, const char* label) {
    std::cout << label << ": size=" << v.size() << " {";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << (i ? "," : "") << v[i];
    }
    std::cout << "}\n";
}

int main() {
    // --- the bug ---
    std::vector<int> bad;
    bad.reserve(5);                     // capacity 5, but size is still 0
    std::cout << "after reserve(5): size=" << bad.size()
              << " capacity=" << bad.capacity() << '\n';
    // Writing bad[i] = i*i here would be undefined behaviour (i >= size()), and
    // because size() is 0 the vector still contains nothing you can iterate.
    print(bad, "bad (never grown)");

    // --- fix 1: push_back grows the size as it fills ---
    std::vector<int> viaPush;
    viaPush.reserve(5);                 // reserve is still worth it: one allocation
    for (int i = 0; i < 5; ++i) {
        viaPush.push_back(i * i);
    }
    print(viaPush, "fix push_back    ");

    // --- fix 2: size it up front, then operator[] is valid ---
    std::vector<int> viaResize(5);      // 5 value-initialised elements exist now
    for (int i = 0; i < 5; ++i) {
        viaResize[i] = i * i;
    }
    print(viaResize, "fix resize + []  ");
    return 0;
}
