#include <array>
#include <iostream>
#include <span>
#include <vector>

int main() {
    // A span is a non-owning view over a contiguous sequence: a pointer plus a
    // size. It copies nothing and owns nothing -- the data lives elsewhere.
    std::vector<int> v{10, 20, 30, 40};
    std::span<int> s(v); // views v's elements, no copy

    std::cout << "size: " << s.size() << '\n';
    std::cout << "first: " << s.front() << ", last: " << s.back() << '\n';
    std::cout << "s[2]: " << s[2] << '\n';

    // Iterating a span is iterating the underlying elements.
    std::cout << "elements:";
    for (int x : s) std::cout << ' ' << x;
    std::cout << '\n';

    // The same span type views a std::array or a C array -- anything contiguous.
    std::array<int, 3> a{1, 2, 3};
    std::span<int> sa(a);
    std::cout << "array view size: " << sa.size() << '\n';

    // A span points AT the data: it stores only data() and size(), so it refers
    // to the same memory as the vector, it does not copy it.
    std::cout << "views the vector's own storage: " << (s.data() == v.data()) << '\n';
    return 0;
}
