#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }
int volume(int w, int h, int d) { return w * h * d; }

int main() {
    using namespace std::placeholders;

    // std::bind fixes some arguments of a callable now and leaves the rest as
    // placeholders (_1, _2, ...) to be supplied at call time. The result is a
    // new callable.

    // Fix the first argument to 10; _1 is whatever you pass when calling.
    auto add10 = std::bind(add, 10, _1);
    std::cout << "add10(5)  = " << add10(5) << "\n";    // 15
    std::cout << "add10(32) = " << add10(32) << "\n";   // 42

    // Fix two of three arguments; one placeholder remains for the call.
    auto areaTimesDepth = std::bind(volume, 4, 5, _1);
    std::cout << "volume(4,5,_1)(3) = " << areaTimesDepth(3) << "\n";   // 60

    // Fix ALL arguments -- a zero-argument callable, sometimes called a thunk.
    auto thunk = std::bind(add, 1, 2);
    std::cout << "thunk() = " << thunk() << "\n";        // 3
    return 0;
}
