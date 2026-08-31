// null_memory_resource() always throws bad_alloc. As the upstream of a fixed
// monotonic buffer it makes that buffer a hard ceiling: what fits is served,
// and the first request that does not fit fails loudly instead of spilling to
// the heap -- an enforced memory budget.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <new>
#include <vector>

int main() {
    std::array<std::byte, 128> buffer;
    // Fixed buffer, and NO fallback: overflow throws instead of hitting new.
    std::pmr::monotonic_buffer_resource res{
        buffer.data(), buffer.size(), std::pmr::null_memory_resource()};

    std::pmr::vector<int> v{&res};

    // Within budget: 128 bytes holds well over 16 ints.
    v.reserve(16);
    for (int i = 0; i < 16; ++i) v.push_back(i);
    std::cout << "reserved 16 ints within the 128-byte budget\n";

    // Over budget: ask for far more than the buffer can hold.
    try {
        std::pmr::vector<int> big{&res};
        big.reserve(1000);              // 4000 bytes > 128: upstream is null
        std::cout << "(not reached)\n";
    } catch (const std::bad_alloc&) {
        std::cout << "over-budget request threw bad_alloc, no heap fallback\n";
    }
}
