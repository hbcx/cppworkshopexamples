#include <iostream>
#include <span>
#include <vector>

// span<int> is a WRITABLE view: assigning through it changes the caller's data.
void double_all(std::span<int> s) {
    for (int& x : s) x *= 2;
}

// span<const int> is read-only: it cannot modify the elements.
long sum(std::span<const int> s) {
    long t = 0;
    for (int x : s) t += x;
    return t;
}

int main() {
    std::vector<int> v{1, 2, 3, 4};

    double_all(v); // writes through the span, in place, into v
    std::cout << "after double:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    // A writable span converts to a read-only one for free (not the reverse).
    std::cout << "sum: " << sum(v) << '\n';

    // Write through just a window: subspan gives a writable view of part of v.
    std::span<int>(v).subspan(1, 2).front() = 99;
    std::cout << "v[1] after windowed write: " << v[1] << '\n';
    return 0;
}
