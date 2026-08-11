#include <iostream>
#include <list>
#include <type_traits>
#include <vector>

// A VALUE trait of your own: is T a std::vector? Default false; a partial
// specialization matches vector<...> and says true.
template <class T>
struct is_std_vector : std::false_type {};
template <class T, class Alloc>
struct is_std_vector<std::vector<T, Alloc>> : std::true_type {};

// Give it the modern _v spelling, like the standard traits (variable template).
template <class T>
constexpr bool is_std_vector_v = is_std_vector<T>::value;

// A TRANSFORMATION trait of your own: the element type of a vector, or the type
// itself if it is not one.
template <class T>
struct element_type { using type = T; };
template <class T, class Alloc>
struct element_type<std::vector<T, Alloc>> { using type = T; };
template <class T>
using element_type_t = typename element_type<T>::type;

int main() {
    std::cout << std::boolalpha;
    std::cout << "is_std_vector<vector<int>> = " << is_std_vector_v<std::vector<int>> << "\n";  // true
    std::cout << "is_std_vector<int>         = " << is_std_vector_v<int> << "\n";               // false
    std::cout << "is_std_vector<list<int>>   = " << is_std_vector_v<std::list<int>> << "\n";     // false

    static_assert(std::is_same<element_type_t<std::vector<double>>, double>::value, "vector<double> element is double");
    static_assert(std::is_same<element_type_t<int>, int>::value, "int stays int");

    std::cout << "custom traits work\n";
    return 0;
}
