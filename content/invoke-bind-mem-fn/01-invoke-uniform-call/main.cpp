#include <functional>
#include <iostream>
#include <utility>

int times2(int x) { return x * 2; }

struct Point {
    int x;
    int y;
    int sum() const { return x + y; }
};

// Generic code cannot just write f(args...) and cover every callable: a
// pointer-to-member is called with obj.*p syntax, not p(obj). std::invoke is the
// ONE rule that covers them all, so a wrapper written once accepts any of them.
template <typename F, typename... Args>
auto callThrough(F&& f, Args&&... args) {
    return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

int main() {
    // A free function and a lambda: invoke calls them the ordinary way.
    std::cout << "free function : " << callThrough(times2, 21) << "\n";                 // 42
    std::cout << "lambda        : " << callThrough([](int a){ return a + 1; }, 41) << "\n"; // 42

    // A functor (an object with operator()).
    struct Add {
        int base;
        int operator()(int a) const { return a + base; }
    };
    std::cout << "functor       : " << callThrough(Add{10}, 32) << "\n";                // 42

    Point p{40, 2};
    // A pointer-to-member-FUNCTION: invoke passes the object as the first arg,
    // then the rest. Writing p(...) here would not compile.
    std::cout << "member func   : " << callThrough(&Point::sum, p) << "\n";             // 42

    // A pointer-to-DATA-member: with one argument invoke is just member access --
    // the same rule, no call at all. This is the case that makes f(args...) fail.
    std::cout << "data member x : " << callThrough(&Point::x, p) << "\n";               // 40
    return 0;
}
