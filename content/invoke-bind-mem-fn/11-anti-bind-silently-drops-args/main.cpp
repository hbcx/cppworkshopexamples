#include <functional>
#include <iostream>

int onlyFirst(int a) { return a; }

int main() {
    using namespace std::placeholders;

    // ANTI-PATTERN: bind silently IGNORES any call argument no placeholder names.
    // Passing more arguments than the placeholders use is not an error -- the
    // extras are evaluated and thrown away, which hides bugs like a wrong
    // callback signature or a mis-typed placeholder.
    auto f = std::bind(onlyFirst, _1);
    std::cout << "f(10, 20, 30) = " << f(10, 20, 30) << "\n";   // 10 -- 20 and 30 dropped

    // The real danger: you meant _1 but wrote _2 (or the wrong number). bind
    // compiles and quietly returns the wrong argument instead of failing.
    auto g = std::bind(onlyFirst, _2);
    std::cout << "g(1, 2)       = " << g(1, 2) << "\n";         // 2, not 1

    // FIX: a lambda has a fixed parameter list, so an extra or missing argument
    // is a COMPILE error -- the exact safety bind gives up.
    auto h = [](int a){ return a; };
    std::cout << "h(10)         = " << h(10) << "\n";           // 10
    // h(10, 20); // would NOT compile -- too many arguments
    return 0;
}
