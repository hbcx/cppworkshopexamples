#include <iostream>
#include <utility>

struct SafeMove {
    SafeMove() = default;
    SafeMove(const SafeMove&)      { std::cout << "  SafeMove copied\n"; }
    SafeMove(SafeMove&&) noexcept  { std::cout << "  SafeMove moved\n"; }
};

struct RiskyMove {
    RiskyMove() = default;
    RiskyMove(const RiskyMove&)    { std::cout << "  RiskyMove copied\n"; }
    RiskyMove(RiskyMove&&)         { std::cout << "  RiskyMove moved (can throw)\n"; }  // NOT noexcept
};

int main() {
    SafeMove a;
    RiskyMove b;

    // move_if_noexcept moves only when the move constructor is noexcept; otherwise
    // it hands back an lvalue so a COPY is made. That keeps the source intact if
    // the operation throws -- the strong exception guarantee.
    std::cout << "SafeMove (noexcept move):\n";
    SafeMove a2 = std::move_if_noexcept(a);    // moved
    std::cout << "RiskyMove (throwing move):\n";
    RiskyMove b2 = std::move_if_noexcept(b);   // copied

    (void)a2;
    (void)b2;
    return 0;
}
