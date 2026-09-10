#include <iostream>
#include <vector>

int g_moves = 0;
int g_copies = 0;

// ANTI-PATTERN: a move constructor that is not marked noexcept. std::vector cannot
// risk it during reallocation -- a throw partway through relocating elements would
// corrupt the container -- so it falls back to COPYING every element when it grows,
// silently discarding the performance a move would have given.
struct Risky {
    Risky() = default;
    Risky(const Risky&)  { ++g_copies; }
    Risky(Risky&&)       { ++g_moves; }            // NOT noexcept -> vector copies
};

// FIX: mark the move noexcept. Now vector trusts it and moves on reallocation.
struct Safe {
    Safe() = default;
    Safe(const Safe&)      { ++g_copies; }
    Safe(Safe&&) noexcept  { ++g_moves; }
};

template <typename T>
void growPastCapacity() {
    std::vector<T> v;
    v.reserve(1);
    v.emplace_back();   // one element, capacity 1
    v.emplace_back();   // reallocates: the existing element must be relocated
}

int main() {
    g_moves = g_copies = 0;
    growPastCapacity<Risky>();
    std::cout << "Risky (throwing move): moves=" << g_moves << " copies=" << g_copies << "\n";

    g_moves = g_copies = 0;
    growPastCapacity<Safe>();
    std::cout << "Safe  (noexcept move): moves=" << g_moves << " copies=" << g_copies << "\n";
    return 0;
}
