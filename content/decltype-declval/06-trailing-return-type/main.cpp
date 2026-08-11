#include <iostream>
#include <string>

// When the return type DEPENDS on the parameters, C++11 cannot write it before
// the function name -- the parameters are not in scope yet. The trailing return
// type puts it AFTER the parameter list, where a and b are visible.
template <class T, class U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// The classic use: a forwarding wrapper whose return type is whatever the wrapped
// call returns.
template <class F, class Arg>
auto callOn(F f, Arg x) -> decltype(f(x)) {
    return f(x);
}

int len(const std::string& s) { return static_cast<int>(s.size()); }

int main() {
    auto a = add(2, 3.5);                               // decltype(int + double) -> double
    auto b = add(std::string("x"), std::string("y"));   // decltype(string + string) -> string
    std::cout << "add: " << a << " / " << b << "\n";

    std::cout << "callOn(len): " << callOn(len, std::string("hello")) << "\n";
    return 0;
}
