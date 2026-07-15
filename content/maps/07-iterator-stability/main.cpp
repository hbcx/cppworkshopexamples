#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};

    // Take a pointer and an iterator to an existing element.
    std::string* p = &m[2];
    auto it = m.find(2);

    // std::map stores each element in its own node, so inserting and erasing
    // OTHER elements never moves element 2 -- the handles stay valid. (A vector
    // would reallocate and invalidate both.)
    for (int k = 4; k < 100; ++k) {
        m[k] = "x";        // many insertions
    }
    m.erase(1);            // erase a different element

    std::cout << "pointer still valid:  *p = " << *p << '\n';
    std::cout << "iterator still valid: it->second = " << it->second << '\n';

    // Only erasing the element itself invalidates handles to it.
    m.erase(2);
    std::cout << "after erasing key 2, size = " << m.size() << '\n';
    return 0;
}
