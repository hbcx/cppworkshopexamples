#include <atomic>
#include <iostream>

// atomic_ref works only on a suitably ALIGNED object, and whether it is lock-free
// depends on the type and that alignment. Two compile-time constants tell you what
// you need: atomic_ref<T>::required_alignment is the alignment the referenced object
// must have (it can be stricter than the type's ordinary alignof), and
// atomic_ref<T>::is_always_lock_free says whether the operations never take a lock.
// You can guarantee a lock-free layout by aligning the object with alignas.
int main() {
    // Give the int exactly the alignment atomic_ref asks for.
    alignas(std::atomic_ref<int>::required_alignment) int value = 0;

    std::atomic_ref<int> ref{value};

    std::cout << std::boolalpha;
    std::cout << "required_alignment = " << std::atomic_ref<int>::required_alignment << '\n';
    std::cout << "is_always_lock_free = " << std::atomic_ref<int>::is_always_lock_free << '\n';
    // (An object also offers a runtime is_lock_free() that agrees, but on some
    //  toolchains it calls into libatomic, which is not linked by default;
    //  is_always_lock_free is a compile-time constant that needs no library.)

    ref.store(7, std::memory_order_relaxed);
    std::cout << "value after atomic store = " << value << '\n';   // 7
    return 0;
}
