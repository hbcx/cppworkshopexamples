#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

// Concepts combine like boolean expressions: && (both), || (either), ! (not),
// and one concept can be defined in terms of another (refinement).
template <class T>
concept Number = std::integral<T> || std::floating_point<T>;              // disjunction

template <class T>
concept SignedNumber = Number<T> && std::is_signed_v<T>;                  // refine + conjunction

template <class T>
concept TextLike = std::convertible_to<T, std::string> && !std::integral<T>;  // negation

template <Number T>   void tag(const T&)   { std::cout << "number\n"; }
template <TextLike T> void tag(const T&)   { std::cout << "text\n"; }

int main() {
    static_assert(Number<int> && Number<double>);
    static_assert(SignedNumber<int> && !SignedNumber<unsigned>);
    static_assert(TextLike<std::string> && !TextLike<int>);

    tag(42);                       // number
    tag(3.5);                      // number
    tag(std::string("hi"));        // text
    std::cout << "done\n";
}
