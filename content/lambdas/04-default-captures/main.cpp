#include <iostream>

int main() {
    int a = 1, b = 2, c = 3;

    // [=] captures everything the body USES, by value; [&] captures by reference.
    auto byValue = [=] { return a + b + c; };            // copies a, b, c
    auto byRef   = [&] { a *= 10; b *= 10; c *= 10; };    // refers to a, b, c

    std::cout << "byValue (snapshot of 1+2+3): " << byValue() << '\n';   // 6
    byRef();
    std::cout << "after byRef, a b c = " << a << ' ' << b << ' ' << c << '\n';  // 10 20 30
    std::cout << "byValue still: " << byValue() << '\n';   // 6 -- it kept its own copies

    // Mixed form: pick a default, then name the exceptions. [&, base] means
    // everything by reference EXCEPT base, which is captured by value.
    int base = 5;
    auto mix = [&, base] { return a + base; };   // a by reference, base by value
    base = 999;
    std::cout << "mix (a by ref = 10, base by value = 5): " << mix() << '\n';   // 15
    return 0;
}
