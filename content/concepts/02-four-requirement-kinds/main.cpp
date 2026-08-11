#include <concepts>
#include <cstddef>
#include <iostream>
#include <vector>

// A requires-expression can hold four kinds of requirement. This one concept
// uses all four to describe "a container we can measure and read the front of".
template <class T>
concept Readable = requires(const T& c) {
    c.begin();                                            // (1) simple: must be valid
    typename T::value_type;                               // (2) type: must name this type
    { c.size() } -> std::convertible_to<std::size_t>;     // (3) compound: valid + result concept
    requires std::default_initializable<typename T::value_type>;  // (4) nested: a concept must hold
};

template <Readable T>
void report(const T& c) {
    std::cout << c.size() << " elements, first = " << *c.begin() << "\n";
}

int main() {
    std::vector<int> v = {10, 20, 30};
    report(v);

    static_assert(Readable<std::vector<int>>);
    static_assert(!Readable<int>);     // int has no begin(), no value_type, no size()

    std::cout << "done\n";
}
