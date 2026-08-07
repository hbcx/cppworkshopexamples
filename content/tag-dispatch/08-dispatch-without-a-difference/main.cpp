#include <iostream>
#include <type_traits>

// BAD: tag dispatch with two overloads that do the SAME work. The dispatch adds
// two functions and an empty-struct hop but branches on nothing that matters --
// both paths just print the value.
template <class T>
void showBad(const T& x, std::true_type)  { std::cout << "value: " << x << "\n"; }
template <class T>
void showBad(const T& x, std::false_type) { std::cout << "value: " << x << "\n"; }
template <class T>
void showBad(const T& x) { showBad(x, std::is_integral<T>{}); }

// GOOD: if the two paths are identical, there is no dispatch to make. One
// function is shorter and clearer.
template <class T>
void showGood(const T& x) { std::cout << "value: " << x << "\n"; }

int main() {
    showBad(42);
    showBad(3.5);
    showGood(42);
    showGood(3.5);

    // Reach for tag dispatch only when the paths genuinely differ AND at least
    // one would not compile for the other types (so a run-time if is out). In
    // C++17 a single function with if constexpr (std::is_integral<T>::value)
    // expresses a real two-way choice in one place and replaces most simple tag
    // dispatch -- this section targets C++11, where tag dispatch is the tool.
    return 0;
}
