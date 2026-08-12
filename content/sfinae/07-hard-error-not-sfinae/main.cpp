#include <iostream>
#include <cstddef>
#include <type_traits>

// SFINAE only recovers from a failure in the IMMEDIATE context of substitution --
// the function's own signature. A failure one level deeper, inside the body of
// another template that substitution triggers, is a hard error: the overload is
// NOT dropped, the whole build breaks.

// A trait that HARD-ERRORS for non-integers instead of being SFINAE-friendly.
template <class T>
struct StrictSize {
    static_assert(std::is_integral<T>::value, "T must be integral");
    static const std::size_t value = sizeof(T);
};

// BAD (shown in a comment -- it would not compile): the enable_if condition
// reaches into StrictSize<T>::value. For a non-integral T the static_assert
// INSIDE StrictSize fires. That failure is in a non-immediate context, so
// instead of quietly removing this overload it stops the compiler, and the call
// does NOT fall through to another overload.
//
//   template <class T>
//   typename std::enable_if<(StrictSize<T>::value > 0), const char*>::type
//   f(T) { return "..."; }
//
//   f(3.14);   // hard error: static_assert failed -- not a dropped overload

// GOOD: keep the condition in the immediate context. is_integral<T>::value is a
// plain bool computed right in the signature, so when it is false this is an
// ordinary substitution failure and the overload simply drops out, letting the
// other one win.
template <class T>
typename std::enable_if<std::is_integral<T>::value, const char*>::type
f(T) { return "integral overload"; }

template <class T>
typename std::enable_if<!std::is_integral<T>::value, const char*>::type
f(T) { return "non-integral overload"; }

int main() {
    std::cout << "f(42)   -> " << f(42) << "\n";
    std::cout << "f(3.14) -> " << f(3.14) << "\n";
    return 0;
}
