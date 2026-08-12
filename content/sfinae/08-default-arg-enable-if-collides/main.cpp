#include <iostream>
#include <type_traits>

// A very common first attempt at two trait-guarded overloads: put enable_if in a
// DEFAULT TYPE parameter. It does not compile, and the error surprises everyone.

// BAD (shown in a comment -- it is a redefinition, not two overloads):
//
//   template <class T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
//   void g(T) { std::cout << "integral\n"; }
//   template <class T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
//   void g(T) { std::cout << "floating point\n"; }
//
// Default template ARGUMENTS are not part of a template's signature, so both
// declarations have the SAME signature -- template <class T, class> void g(T) --
// and the compiler rejects the second as a redefinition of the first.

// GOOD: make the enable_if a non-type parameter that carries a value (= 0). Now
// the parameter's TYPE (not its default) differs between the two -- one is
// enable_if<...integral...>::type, the other enable_if<...floating...>::type --
// so the templates have different parameter lists and are genuinely distinct.
template <class T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void g(T) { std::cout << "integral\n"; }

template <class T,
          typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
void g(T) { std::cout << "floating point\n"; }

int main() {
    g(42);
    g(3.14);
    return 0;
}
