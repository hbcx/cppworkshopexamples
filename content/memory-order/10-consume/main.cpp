#include <atomic>
#include <iostream>
#include <thread>

// Anti-pattern: reaching for memory_order_consume.
//
// consume was meant to be a cheaper acquire that orders only operations DEPENDENT on
// the loaded value (following the loaded pointer, say), rather than everything after
// the load. In practice its rules proved so hard to specify and implement that every
// major compiler simply PROMOTES it to acquire -- so it buys nothing today, and the
// standard discourages its use while its definition is being reworked:
//
//   const int* p = ptr.load(std::memory_order_consume);   // treated as acquire anyway
//
// The fix is to write what you mean: memory_order_acquire. It is correct, well
// understood, and no slower than the promoted consume. Below a pointer is published
// with release and read with acquire.
static const int value = 7;             // lives for the whole program

int main() {
    std::atomic<const int*> ptr{nullptr};

    std::jthread consumer([&] {
        const int* p = nullptr;
        while (!(p = ptr.load(std::memory_order_acquire))) {   // acquire, not consume
        }
        std::cout << "consumer followed the pointer to " << *p << '\n';   // 7
    });

    ptr.store(&value, std::memory_order_release);   // publish the pointer
    return 0;
}
