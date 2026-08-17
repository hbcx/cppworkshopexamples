#include <atomic>
#include <iostream>
#include <memory>

// A surprise worth knowing: std::atomic<std::shared_ptr<T>> is usually NOT lock-free.
// It has to update two things together -- the pointer and the control block's
// reference count -- so the implementation guards them with an internal lock (often a
// small pool of spinlocks keyed by address). is_always_lock_free reflects this and is
// defined to be false for this specialization. That is fine: it is still far cheaper
// than a mutex around the whole object for a read-mostly workload, and correctness
// does not depend on being lock-free. The full memory_order set works on
// load/store/exchange, just like any other atomic.
int main() {
    std::atomic<std::shared_ptr<int>> p{std::make_shared<int>(1)};

    std::cout << std::boolalpha;
    std::cout << "atomic<shared_ptr> is_always_lock_free = "
              << std::atomic<std::shared_ptr<int>>::is_always_lock_free << '\n';   // false

    // Publish with release, read with acquire -- the same ordering model as any atomic.
    p.store(std::make_shared<int>(2), std::memory_order_release);
    std::shared_ptr<int> snap = p.load(std::memory_order_acquire);
    std::cout << "value after release/acquire store = " << *snap << '\n';   // 2
    return 0;
}
