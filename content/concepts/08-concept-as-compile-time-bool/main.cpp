#include <concepts>
#include <iostream>
#include <string>

template <class T>
concept Printable = requires(const T& x, std::ostream& os) {
    { os << x } -> std::same_as<std::ostream&>;
};

// A concept is a compile-time bool, so it can drive `if constexpr` inside ONE
// function -- no second overload needed. The untaken branches are discarded, so
// each branch need only be valid for the types that actually reach it.
template <class T>
void describe(const T& x) {
    if constexpr (std::integral<T>) {
        std::cout << "integer: " << x << "\n";
    } else if constexpr (Printable<T>) {
        std::cout << "printable: " << x << "\n";
    } else {
        std::cout << "opaque value of size " << sizeof(T) << "\n";
    }
}

struct Opaque { int a; int b; };   // no operator<<

int main() {
    describe(42);                    // integer
    describe(std::string("hi"));     // printable
    describe(Opaque{});              // opaque (the << branch is never instantiated for it)
    std::cout << "done\n";
}
