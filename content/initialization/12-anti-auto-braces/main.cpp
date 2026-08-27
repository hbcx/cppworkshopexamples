#include <iostream>
#include <initializer_list>
#include <type_traits>

// Anti-pattern: assuming auto x{value} deduces the value's type. With auto, the
// brace forms follow a special rule (settled in C++17): a SINGLE braced value
// gives that value's type, but "= {list}" gives std::initializer_list, and a
// direct-brace list with more than one element is ill-formed. Get it wrong and
// you hold an initializer_list where you expected an int.

int main() {
    // Single value, direct braces: this is int (the C++17 rule).
    auto a{5};
    std::cout << "auto a{5}        is int? "
              << std::is_same<decltype(a), int>::value << "\n";            // 1

    // "= {single}" is STILL std::initializer_list<int>, not int:
    auto b = {5};
    std::cout << "auto b = {5}     is initializer_list? "
              << std::is_same<decltype(b), std::initializer_list<int>>::value
              << "\n";                                                     // 1

    // "= {list}" is std::initializer_list<int> -- fine when you want one:
    auto c = {1, 2, 3};
    std::cout << "auto c = {1,2,3} size = " << c.size() << "\n";           // 3

    //     auto d{1, 2};   // ERROR in C++17: direct braces need exactly one value
    //
    // and before C++17 even auto a{5} was initializer_list<int>, not int -- that
    // is the rule that changed.

    // The habit: for a plain variable, prefer auto x = value; use auto x{value}
    // only when you know it is a single value and you want its exact type.
    auto plain = 5;
    std::cout << "auto plain = 5   is int? "
              << std::is_same<decltype(plain), int>::value << "\n";        // 1
    return 0;
}
