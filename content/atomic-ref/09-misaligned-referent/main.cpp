#include <atomic>
#include <cstdint>
#include <iostream>

// Anti-pattern: constructing an atomic_ref on an under-aligned object.
//
// atomic_ref requires the referenced object to be aligned to
// atomic_ref<T>::required_alignment, which can be STRICTER than the type's ordinary
// alignof -- a lock-free 64-bit access may need 8-byte alignment that a byte buffer
// or a packed struct does not provide. Building an atomic_ref on an object that does
// not meet required_alignment is undefined behaviour:
//
//   alignas(1) unsigned char buf[sizeof(std::int64_t)];   // 1-byte aligned storage
//   auto* p = reinterpret_cast<std::int64_t*>(buf);        // may be under-aligned
//   std::atomic_ref<std::int64_t> ref{*p};                 // UB if &*p is not aligned
//   // We do not run that.
//
// The fix is to give the object the alignment atomic_ref asks for -- with alignas, or
// by laying the data out so every atomically-accessed field is naturally aligned.
// Below the value is aligned to exactly required_alignment, so the ref is well-defined.
int main() {
    constexpr std::size_t need = std::atomic_ref<std::int64_t>::required_alignment;

    alignas(need) std::int64_t value = 0;      // meets the requirement by construction
    std::atomic_ref<std::int64_t> ref{value};

    ref.fetch_add(21, std::memory_order_relaxed);
    ref.fetch_add(21, std::memory_order_relaxed);

    std::cout << "required_alignment = " << need
              << ", aligned value after atomic adds = " << value << '\n';   // 42
    return 0;
}
