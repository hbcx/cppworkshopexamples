// Anti-pattern: assuming std::pmr::vector and std::vector are interchangeable.
//
// They differ in the allocator (std::allocator vs polymorphic_allocator), so
// they are different types: one does not pass where the other is expected, and
// crossing between them copies elements rather than moving the buffer.

#include <iostream>
#include <memory_resource>
#include <type_traits>
#include <vector>

// This takes a plain std::vector -- a pmr::vector will not bind to it.
static long sum(const std::vector<int>& v) {
    long s = 0;
    for (int x : v) s += x;
    return s;
}

int main() {
    std::cout << std::boolalpha;
    std::cout << "std::pmr::vector<int> is std::vector<int>? "
              << std::is_same<std::pmr::vector<int>, std::vector<int>>::value << '\n';

    std::pmr::vector<int> p{1, 2, 3, 4};

    // BAD (does not compile -- described):
    //   sum(p);   // no known conversion from pmr::vector<int> to std::vector<int>

    // GOOD: bridge deliberately by copying the elements.
    std::vector<int> plain(p.begin(), p.end());
    std::cout << "converted by copy, sum = " << sum(plain) << '\n';
}
