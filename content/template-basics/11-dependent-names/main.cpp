#include <iostream>
#include <vector>

// Inside a template, a name that depends on a template parameter is a DEPENDENT
// name. The compiler does not know if it is a type or a value until instantiation,
// so it assumes a value by default -- you write `typename` to say it is a type.
template <class Container>
typename Container::value_type firstOf(const Container& c) {
    // `typename` says Container::const_iterator is a TYPE. Without it, the
    // compiler assumes a value and rejects the declaration.
    typename Container::const_iterator it = c.begin();
    return *it;
}

// The return type Container::value_type is dependent too, so it also needs
// typename. sumOf shows the same rule in a second spot.
template <class Container>
typename Container::value_type sumOf(const Container& c) {
    typename Container::value_type total{};
    for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
        total += *it;
    return total;
}

int main() {
    std::vector<int> v = {7, 8, 9};
    std::cout << "firstOf(vector) = " << firstOf(v) << "\n";   // 7
    std::cout << "sumOf(vector)   = " << sumOf(v) << "\n";     // 24
    return 0;
}
