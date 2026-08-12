#include <iostream>

int main() {
    // Normally the return type is deduced from the return statement, like auto.
    auto add = [](int a, int b) { return a + b; };   // deduces int
    std::cout << "add(2,3) = " << add(2, 3) << '\n';

    // A trailing -> Type is needed when different return statements would deduce
    // different types, or when you want a specific type. Without -> double here,
    // the int branch and the double branch would disagree and fail to compile.
    auto half = [](int x) -> double {
        if (x % 2 == 0) return x / 2;    // an int...
        else           return x / 2.0;  // ...and a double
    };
    std::cout << "half(4) = " << half(4) << ", half(5) = " << half(5) << '\n';
    return 0;
}
