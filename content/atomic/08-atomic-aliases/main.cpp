#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;

    // <atomic> defines a set of convenience typedefs so you can write std::atomic_int
    // instead of std::atomic<int>. They are NOTHING MORE than aliases -- the same
    // type, not a different or "better" atomic. There is one for each common scalar:
    //   atomic_bool, atomic_char, atomic_int, atomic_uint, atomic_long,
    //   atomic_llong, atomic_size_t, atomic_intptr_t, ... and many more.
    std::atomic_int  counter{0};      // exactly std::atomic<int>
    std::atomic_bool ready{false};    // exactly std::atomic<bool>

    counter.fetch_add(5);
    ready.store(true);
    std::cout << "counter = " << counter.load() << '\n';   // 5
    std::cout << "ready   = " << ready.load() << '\n';      // true

    // Proof that each alias is the very same type as the spelled-out template.
    // is_same<A, B>::value is a compile-time bool, so this is checked by the compiler.
    std::cout << "atomic_int    == atomic<int>?    "
              << std::is_same<std::atomic_int,    std::atomic<int> >::value << '\n';
    std::cout << "atomic_bool   == atomic<bool>?   "
              << std::is_same<std::atomic_bool,   std::atomic<bool> >::value << '\n';
    std::cout << "atomic_size_t == atomic<size_t>? "
              << std::is_same<std::atomic_size_t, std::atomic<std::size_t> >::value << '\n';
    std::cout << "atomic_llong  == atomic<long long>? "
              << std::is_same<std::atomic_llong,  std::atomic<long long> >::value << '\n';

    // One trap: std::atomic_flag is NOT one of these aliases. It is a SEPARATE type
    // (not std::atomic<bool>) -- the only atomic guaranteed lock-free on every
    // platform, with just test_and_set/clear and no load or store. Do not reach for
    // it expecting an atomic<bool>; use atomic_bool for a normal boolean flag.
    std::cout << "atomic_flag   == atomic<bool>?   "
              << std::is_same<std::atomic_flag,   std::atomic<bool> >::value << '\n';  // false

    // Bottom line: the aliases are pure spelling sugar. Pick whichever reads better;
    // atomic<int> and atomic_int compile to the identical thing.
    return 0;
}
