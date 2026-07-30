#include <iostream>
#include <string>
#include <utility>

int main() {
    // make_pair deduces both types from the arguments, so you don't repeat them.
    auto p = std::make_pair(1, 3.5);          // pair<int, double>
    std::cout << p.first << " / " << p.second << '\n';

    // The equivalent written out in full -- same object, more typing.
    std::pair<int, double> q(1, 3.5);
    std::cout << "equal? " << (p == q ? "yes" : "no") << '\n';

    // make_pair DECAYS its arguments: a string literal becomes const char*,
    // not std::string. So this pair holds a pointer, and calling .first.size()
    // would not compile.
    auto raw = std::make_pair("hello", 5);    // pair<const char*, int>
    std::cout << "decayed to C string: " << raw.first << '\n';

    // To store a std::string, name the type or build the string explicitly.
    auto text = std::make_pair(std::string("hello"), 5);  // pair<std::string, int>
    std::cout << "length = " << text.first.size() << '\n'; // .size() works now

    // (C++17 adds class template argument deduction: std::pair r{1, 3.5};
    //  builds the same pair<int,double> without make_pair.)
    return 0;
}
