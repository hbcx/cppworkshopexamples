#include <iostream>
#include <string>
#include <tuple>

int add3(int a, int b, int c) { return a + b + c; }

void describe(const std::string& who, int age) {
    std::cout << who << " is " << age << '\n';
}

int main() {
    // std::apply (C++17) calls a function using a tuple's elements as its
    // arguments -- it unpacks the tuple into the call for you.
    auto args = std::make_tuple(1, 2, 3);
    std::cout << "sum = " << std::apply(add3, args) << '\n';

    // The classic use: a set of arguments stored now and invoked later, without
    // hand-writing get<0>, get<1>, ... at the call site.
    auto bound = std::make_tuple(std::string("Ada"), 36);
    std::apply(describe, bound);
    return 0;
}
