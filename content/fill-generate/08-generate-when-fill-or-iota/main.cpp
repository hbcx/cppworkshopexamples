#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>

int main() {
    // BAD: generate with a lambda that returns a CONSTANT -- this is just fill,
    // written with more machinery.
    std::vector<int> a(5);
    std::generate(a.begin(), a.end(), []() { return 42; });   // fill in disguise

    // BAD: generate with a captured counter to make 0 1 2 3 4 -- this is iota,
    // with an external counter to declare, initialize, and keep in sync.
    std::vector<int> b(5);
    int i = 0;
    std::generate(b.begin(), b.end(), [&i]() { return i++; });  // iota in disguise

    // GOOD: fill states "every element is this value"; iota states "consecutive
    // values from here". Each names the intent and needs no captured state.
    std::vector<int> fa(5);
    std::fill(fa.begin(), fa.end(), 42);
    std::vector<int> fb(5);
    std::iota(fb.begin(), fb.end(), 0);

    std::cout << "fill matches? " << std::boolalpha << (a == fa) << '\n';
    std::cout << "iota matches? " << (b == fb) << '\n';
    return 0;
}
