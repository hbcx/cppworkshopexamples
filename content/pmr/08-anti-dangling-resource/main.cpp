// Anti-pattern: letting the memory_resource die while the container lives on.
//
// A pmr container only stores a pointer to its resource. If the resource (or its
// buffer) is destroyed first, later allocations and the final cleanup are
// undefined. The rule: the resource must outlive every container using it.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <vector>

// BAD (dangling -- described, not run):
//   std::pmr::vector<int> makeVec() {
//       std::array<std::byte, 256> buf;                 // local buffer
//       std::pmr::monotonic_buffer_resource res{buf.data(), buf.size()};
//       std::pmr::vector<int> v{&res};
//       v.push_back(1);
//       return v;   // buf and res die here; the returned vector dangles
//   }

// GOOD: the caller owns the resource, so it outlives the container.
static void fill(std::pmr::vector<int>& v) {
    for (int i = 0; i < 5; ++i) v.push_back(i * 3);
}

int main() {
    std::array<std::byte, 256> buffer;
    std::pmr::monotonic_buffer_resource res{buffer.data(), buffer.size()};

    // res is declared before v, so v is destroyed first -- resource outlives it.
    std::pmr::vector<int> v{&res};
    fill(v);

    std::cout << "vector on a caller-owned resource:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
    std::cout << "resource outlives the container, so cleanup is safe\n";
}
