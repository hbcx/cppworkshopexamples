#include <iostream>
#include <type_traits>
#include <vector>

struct Safe {
    Safe() = default;
    Safe(Safe&&) noexcept { std::cout << "move "; }
    Safe(const Safe&) { std::cout << "copy "; }
};

struct Unsafe {
    Unsafe() = default;
    Unsafe(Unsafe&&) { std::cout << "move "; }   // NOT noexcept
    Unsafe(const Unsafe&) { std::cout << "copy "; }
};

// Put two elements into a vector sized for one, forcing a reallocation that
// must relocate the first element. Watch whether it is moved or copied.
template <typename T>
void grow() {
    std::vector<T> v;
    v.reserve(1);
    v.emplace_back();  // first element
    v.emplace_back();  // capacity exceeded -> reallocation relocates the first
}

int main() {
    std::cout << std::boolalpha
              << "Safe   nothrow-move-constructible: "
              << std::is_nothrow_move_constructible<Safe>::value << '\n'
              << "Unsafe nothrow-move-constructible: "
              << std::is_nothrow_move_constructible<Unsafe>::value << '\n';

    std::cout << "vector<Safe> relocation:   ";
    grow<Safe>();    // "move": move ctor is noexcept, so the vector moves
    std::cout << '\n';

    std::cout << "vector<Unsafe> relocation: ";
    grow<Unsafe>();  // "copy": move might throw, so the vector copies instead
    std::cout << '\n';
    return 0;
}
