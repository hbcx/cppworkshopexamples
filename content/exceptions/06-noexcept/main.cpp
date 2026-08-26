// noexcept declares that a function will not throw. It is a promise the compiler
// and library build on -- most importantly, containers move (instead of copy)
// elements only when the move constructor is noexcept. The noexcept OPERATOR
// asks, at compile time, whether an expression can throw.
#include <iostream>
#include <stdexcept>
#include <type_traits>

int safe_add(int a, int b) noexcept {           // promises not to throw
    return a + b;
}

int may_throw(int x) {                           // makes no such promise
    if (x < 0) throw std::runtime_error("negative");
    return x;
}

struct Movable {
    Movable() = default;
    Movable(Movable&&) noexcept {}               // noexcept move -> vector uses it
    Movable(const Movable&) {}
};

int main() {
    std::cout << std::boolalpha;

    // The noexcept operator reports whether a call can throw. It is a
    // compile-time query -- it does not run the function.
    std::cout << "safe_add is noexcept:  " << noexcept(safe_add(1, 2)) << '\n';
    std::cout << "may_throw is noexcept: " << noexcept(may_throw(1)) << '\n';

    // This is the property std::vector checks before moving on reallocation.
    std::cout << "Movable move is noexcept: "
              << std::is_nothrow_move_constructible<Movable>::value << '\n';

    // NOTE: if a function marked noexcept throws anyway, the program does not
    // unwind and you cannot catch it -- std::terminate runs at once. So promise
    // noexcept only when it is true. (Not triggered here.)
    std::cout << "done\n";
    return 0;
}
