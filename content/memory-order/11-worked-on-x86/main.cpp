#include <atomic>
#include <iostream>
#include <thread>

// Anti-pattern: relying on your CPU's strong ordering instead of the C++ memory model.
//
// x86 has a strongly-ordered memory model (total store order), so many
// under-synchronized programs -- a missing release/acquire, or relaxed where a handoff
// was needed -- happen to WORK on x86 and then break on ARM, POWER, or after a compiler
// optimization changes the layout:
//
//   ready.store(true, std::memory_order_relaxed);   // wrong, but x86 rarely reveals it
//
// "It passed on my laptop" proves nothing about a data race. Correctness is defined by
// the C++ memory model, not by the hardware you happened to test on. Reason with
// happens-before, choose the order the ALGORITHM needs, and let ThreadSanitizer -- which
// models the abstract machine, not just x86 -- check it. Below the handoff carries the
// release/acquire the model requires, so it is correct on every target.
int main() {
    int payload = 0;
    std::atomic<bool> ready{false};

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {
        }
        std::cout << "consumer read payload = " << payload << '\n';   // 99
    });

    payload = 99;
    ready.store(true, std::memory_order_release);   // the order the model needs, not what x86 forgives
    return 0;
}
