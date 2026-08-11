#include <iostream>
#include <type_traits>

// std::type_identity_t<T> is just T -- but because T appears in a NON-DEDUCED
// position (behind the alias), the compiler will NOT deduce it from the argument.
// This forces the caller to pin T, so a mixed call cannot pick the wrong type.
template <class T>
T clampToType(std::type_identity_t<T> value) { return value; }

// Contrast: a plain deduced parameter.
template <class T>
T identity(T value) { return value; }

int main() {
    // identity deduces T from the argument:
    std::cout << "identity(42)        = " << identity(42) << "\n";        // T = int

    // clampToType does NOT deduce T -- you must state it, and the argument then
    // converts to that type. That is the point: YOU fix T, not the call.
    std::cout << "clampToType<double> = " << clampToType<double>(42) << "\n";   // int 42 -> double 42
    // clampToType(42);   // ERROR: T is not deducible here -- you must write <T>

    // type_identity is also a plain transformation trait: its ::type is T unchanged.
    static_assert(std::is_same_v<std::type_identity_t<int>, int>);

    std::cout << "done\n";
    return 0;
}
