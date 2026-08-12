#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <utility>
#include <cstddef>

// A detector earns its keep when you ACT on the answer. Here we call reserve() on
// a container only if it has one -- std::vector does, std::list does not -- and
// route the two cases through tag dispatch on the trait's true_type/false_type.

template <class T, class = void>
struct has_reserve : std::false_type {};

template <class T>
struct has_reserve<T, std::void_t<decltype(std::declval<T&>().reserve(std::size_t{}))>>
    : std::true_type {};

// Two implementations selected by a tag argument -- the trait's result type.
template <class C>
void reserveImpl(C& c, std::size_t n, std::true_type) {
    c.reserve(n);
    std::cout << "reserved " << n << "\n";
}
template <class C>
void reserveImpl(C&, std::size_t, std::false_type) {
    std::cout << "reserve not supported -- skipped\n";
}

// The public entry point picks the tag from the trait and dispatches.
template <class C>
void maybeReserve(C& c, std::size_t n) {
    reserveImpl(c, n, has_reserve<C>{});
}

int main() {
    std::vector<int> v;
    std::list<int> l;
    maybeReserve(v, 100);   // vector has reserve  -> reserved
    maybeReserve(l, 100);   // list has none       -> skipped
    return 0;
}
