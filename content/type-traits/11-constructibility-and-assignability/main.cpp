#include <iostream>
#include <string>
#include <type_traits>

// A move-only type: move members defaulted, copy members deleted.
struct Movable {
    Movable() = default;
    Movable(Movable&&) noexcept = default;
    Movable& operator=(Movable&&) noexcept = default;
    Movable(const Movable&) = delete;
    Movable& operator=(const Movable&) = delete;
};

int main() {
    std::cout << std::boolalpha;

    // Plain form: is the operation possible at all?
    static_assert(std::is_default_constructible_v<std::string>);
    static_assert(std::is_copy_constructible_v<std::string>);
    static_assert(std::is_move_constructible_v<Movable>);
    static_assert(!std::is_copy_constructible_v<Movable>);        // copy is deleted
    static_assert(std::is_destructible_v<std::string>);

    // Assignment mirrors construction.
    static_assert(std::is_move_assignable_v<Movable>);
    static_assert(!std::is_copy_assignable_v<Movable>);

    // Nothrow form: a vector uses is_nothrow_move_constructible to decide whether
    // it may move elements on reallocation.
    static_assert(std::is_nothrow_move_constructible_v<Movable>);
    static_assert(std::is_nothrow_move_assignable_v<Movable>);

    // Trivially form: is the operation a simple bit copy?
    static_assert(std::is_trivially_copyable_v<int>);
    static_assert(!std::is_trivially_copyable_v<std::string>);    // manages a heap buffer

    std::cout << "constructibility and assignability traits hold\n";
    return 0;
}
