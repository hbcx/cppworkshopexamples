#include <iostream>

// Tag types: empty structs whose only job is to select an overload.
struct use_fast {};
struct use_safe {};

// Two overloads, chosen by the TYPE of the tag, not by any run-time value.
int compute(int x, use_fast) { std::cout << "fast path\n"; return x << 1; }
int compute(int x, use_safe) { std::cout << "safe path\n"; return x * 2; }

// A generic front-end forwards a tag given as a template argument. Overload
// resolution picks the matching compute() at COMPILE TIME -- no branch remains.
template <class Tag>
int run(int x) { return compute(x, Tag{}); }

int main() {
    std::cout << "run<use_fast>: " << run<use_fast>(21) << "\n";
    std::cout << "run<use_safe>: " << run<use_safe>(21) << "\n";
    return 0;
}
