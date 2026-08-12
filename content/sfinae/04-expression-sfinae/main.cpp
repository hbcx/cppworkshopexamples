#include <iostream>
#include <string>

struct Widget {
    std::string toString() const { return "Widget"; }
};

// EXPRESSION SFINAE: the probe is a whole expression, not just a type. The
// trailing return type -> decltype(x.toString()) is substituted with the real T;
// if T has no toString(), the expression is ill-formed and this overload is
// removed from the set.
template <class T>
auto serialize(const T& x) -> decltype(x.toString()) {
    return x.toString();
}

// #2: the fallback for arithmetic types. Viable only when std::to_string(x) is a
// valid call, which it is for numbers and is not for Widget.
template <class T>
auto serialize(const T& x) -> decltype(std::to_string(x)) {
    return std::to_string(x);
}

int main() {
    Widget w;
    std::cout << "serialize(Widget) -> " << serialize(w) << "\n";   // #1
    std::cout << "serialize(42)     -> " << serialize(42) << "\n";  // #2
    std::cout << "serialize(3.5)    -> " << serialize(3.5) << "\n"; // #2
    return 0;
}
