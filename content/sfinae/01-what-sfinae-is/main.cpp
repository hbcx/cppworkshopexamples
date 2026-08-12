#include <iostream>
#include <vector>

// SUBSTITUTION FAILURE IS NOT AN ERROR (SFINAE). When the compiler builds the
// overload set and substituting the argument types into a template makes its
// signature ill-formed, it does not stop with an error -- it quietly drops that
// one candidate and carries on with the others.

// #1: viable only if T has a nested type ::value_type (containers do, int does
// not). The defaulted third parameter is where the substitution happens. The
// second parameter is a plain int so that, called with 0, this overload is a
// better match than the variadic fallback below.
template <class T>
void describe(const T&, int, typename T::value_type* = nullptr) {
    std::cout << "container (has value_type)\n";
}

// #2: the fallback. The `...` parameter accepts any arguments but ranks WORST in
// overload resolution, so it wins only when #1 was removed by substitution
// failure -- not because the compiler errored, but because #1 stopped existing.
template <class T>
void describe(const T&, ...) {
    std::cout << "plain value (no value_type)\n";
}

int main() {
    std::vector<int> v{1, 2, 3};
    describe(v, 0);    // vector::value_type is int -> #1 viable and preferred
    describe(42, 0);   // int::value_type is ill-formed -> #1 dropped -> #2
    describe(3.14, 0); // same -> #2
    return 0;
}
