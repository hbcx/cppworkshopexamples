#include <iostream>
#include <type_traits>

// A function that strips a reference from a type. It MUST use ::type (or the _t
// helper) -- the trait itself is not the type.
template <class T>
auto stripRef() {
    using Bare = std::remove_reference_t<T>;   // GOOD: _t gives the TYPE (e.g. int)
    // using Bare = std::remove_reference<T>;   // BAD: this names the trait STRUCT,
    //                                          // not int -- a compile error when used
    return Bare{};
}

int main() {
    std::cout << std::boolalpha;

    // Value trait: state the bool you mean with ::value or _v. (is_integral<int>
    // is a class; it happens to convert to bool, which hides the mistake -- so
    // always write ::value or _v to be clear and correct.)
    std::cout << "is_integral_v<int> = " << std::is_integral_v<int> << "\n";   // true
    static_assert(std::is_integral_v<int>);            // clear
    static_assert(std::is_integral<int>::value);       // same thing, spelled out

    // Type trait: forgetting ::type / _t is a hard error, because the trait is a
    // holder, not the type inside it.
    auto x = stripRef<int&>();
    static_assert(std::is_same_v<decltype(x), int>);   // stripRef returned an int
    std::cout << "stripRef<int&>() returned an int: " << x << "\n";   // 0

    std::cout << "done\n";
    return 0;
}
