#include <iostream>
#include <vector>
#include <type_traits>
#include <utility>

// The detection idiom packages expression SFINAE into a reusable yes/no trait. A
// primary template answers "no"; a partial specialization -- viable only when a
// probe expression is well-formed -- answers "yes". std::void_t maps ANY valid
// type to void, so the specialization matches exactly when the decltype compiles.

// Primary: the fallback, used when the specialization does not apply.
template <class T, class = void>
struct has_size : std::false_type {};

// Specialization: viable only if declval<T>().size() is a valid expression. When
// it is, void_t<...> is void and this matches the primary's second argument
// (which defaults to void); when it is not, this specialization is removed and
// the primary's false_type stands.
template <class T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

int main() {
    std::cout << std::boolalpha;
    std::cout << "has_size<std::vector<int>> = " << has_size<std::vector<int>>::value << "\n";
    std::cout << "has_size<int>              = " << has_size<int>::value << "\n";
    return 0;
}
