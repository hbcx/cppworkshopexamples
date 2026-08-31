// Anti-pattern: using a monotonic_buffer_resource for steady allocate/free work.
//
// monotonic's deallocate does nothing, so a loop that allocates then frees walks
// the cursor to the end and never reclaims -- the buffer runs out even though at
// most one block is live. A pool resource reuses freed blocks and keeps going.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <new>

// Allocate then immediately free, over and over. Returns how many iterations
// completed before an allocation failed (or `iterations` if all succeeded).
static int churn(std::pmr::memory_resource* r, int iterations, std::size_t block) {
    const std::size_t al = alignof(std::max_align_t);
    for (int i = 0; i < iterations; ++i) {
        try {
            void* p = r->allocate(block, al);
            r->deallocate(p, block, al);   // monotonic: no-op; pool: reused
        } catch (const std::bad_alloc&) {
            return i;
        }
    }
    return iterations;
}

int main() {
    const int N = 1000;
    const std::size_t block = 64;

    std::array<std::byte, 256> buffer;
    std::pmr::monotonic_buffer_resource mono{
        buffer.data(), buffer.size(), std::pmr::null_memory_resource()};
    int monoDone = churn(&mono, N, block);

    std::pmr::unsynchronized_pool_resource pool{std::pmr::new_delete_resource()};
    int poolDone = churn(&pool, N, block);

    std::cout << "monotonic finished all " << N << " iterations? "
              << std::boolalpha << (monoDone == N)
              << "  (ran out early: " << (monoDone < N) << ")\n";
    std::cout << "pool finished all " << N << " iterations? "
              << (poolDone == N) << '\n';
}
