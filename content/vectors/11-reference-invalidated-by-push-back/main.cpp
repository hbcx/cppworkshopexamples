#include <iostream>
#include <vector>

int main() {
    std::vector<int> data{10, 20, 30};   // size == capacity == 3

    // Anti-pattern: hold a pointer/reference/iterator to an element, then do
    // something that can grow the vector. push_back reallocates here (it is full),
    // leaving the saved handle dangling -- reading it afterwards is undefined
    // behaviour. We only COMPARE the pointers below; we never read through the
    // stale one.
    const int* saved = &data[0];
    data.push_back(40);                  // full -> reallocation
    std::cout << "saved pointer still valid? "
              << (saved == &data[0] ? "yes" : "no")
              << "  (do not read through it either way)\n";

    // Fix 1: reserve enough capacity first, so no reallocation happens while the
    // reference is alive.
    {
        std::vector<int> v{10, 20, 30};
        v.reserve(4);
        int& first = v[0];
        v.push_back(40);                 // capacity 4 was enough: no reallocation
        std::cout << "fix reserve: first still valid = " << first << '\n';
    }

    // Fix 2: do not keep a handle across the mutation -- copy the value out (or
    // re-index by position after the push_back).
    {
        std::vector<int> v{10, 20, 30};
        int firstValue = v[0];           // a copy, not a reference
        v.push_back(40);
        std::cout << "fix copy value: first = " << firstValue << '\n';
    }
    return 0;
}
