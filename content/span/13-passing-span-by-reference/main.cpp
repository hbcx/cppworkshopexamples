#include <iostream>
#include <span>
#include <vector>

// Anti-pattern: passing a span by const reference. A span is already a pointer
// and a size, so a reference just adds an indirection for no saving.
long sum_by_ref(const std::span<const int>& s) { // needless reference
    long t = 0;
    for (int x : s) t += x;
    return t;
}

// Idiomatic: take the span by value.
long sum_by_value(std::span<const int> s) {
    long t = 0;
    for (int x : s) t += x;
    return t;
}

int main() {
    std::vector<int> v{1, 2, 3, 4};

    std::cout << "by ref:   " << sum_by_ref(v) << '\n';
    std::cout << "by value: " << sum_by_value(v) << '\n';

    // Nothing here is worth passing by reference.
    std::cout << "sizeof(span<const int>): " << sizeof(std::span<const int>) << " bytes\n";
    return 0;
}
