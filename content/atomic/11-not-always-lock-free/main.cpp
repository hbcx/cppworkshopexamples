#include <atomic>
#include <iostream>

struct BigState {
    long data[8];   // 64 bytes -- far too large for a lock-free atomic
};

int main() {
    std::cout << std::boolalpha;

    // ANTI-PATTERN: assuming every std::atomic<T> is lock-free. It is not. For
    // small types (int, pointers) the compiler uses real lock-free CPU
    // instructions. For larger types it falls back to an INTERNAL LOCK (a mutex
    // or spinlock from a global table), and its operations even call into a
    // support library (libatomic) -- so it blocks and is not lock-free at all.

    // is_always_lock_free is a compile-time constant: true only when the type is
    // guaranteed lock-free on this platform. It needs no atomic object, so you can
    // query (and static_assert) it without paying for the big type at all.
    std::cout << "atomic<int>      always lock-free? "
              << std::atomic<int>::is_always_lock_free << '\n';        // true
    std::cout << "atomic<BigState> always lock-free? "
              << std::atomic<BigState>::is_always_lock_free << '\n';   // false

    // (An actual object also offers a runtime is_lock_free() that agrees; it is
    // omitted here only because it calls into the libatomic support library, which
    // not every toolchain links by default. is_always_lock_free needs no library.)

    // When a type MUST be lock-free (e.g. to touch it from a signal handler, where
    // taking a lock is undefined), assert it:
    static_assert(std::atomic<int>::is_always_lock_free,
                  "expected lock-free atomic<int>");

    // Why it matters: a "lock-free" atomic that is secretly locking blocks like a
    // mutex, cannot be used in a signal handler, and gives none of the progress
    // guarantees you reached for atomics to get. If you need lock-free, CHECK --
    // keep the shared type small (a pointer, an index) or redesign.
    return 0;
}
