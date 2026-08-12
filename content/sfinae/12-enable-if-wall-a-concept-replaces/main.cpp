#include <iostream>
#include <type_traits>

// The summary anti-pattern: everything in this chapter works, but in C++20 you
// would not write it. enable_if has two lasting costs a concept removes -- it is
// hard to READ, and when nothing matches the compiler says only "no matching
// function" with a wall of candidates, never WHY each one was rejected.

// A realistic constraint: accept a signed integer that is not bool and not char.
template <class T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    !std::is_same<T, bool>::value &&
    !std::is_same<T, char>::value,
    T>::type
absValue(T x) { return x < 0 ? -x : x; }

// The C++20 equivalent is one readable line, and a wrong call names the failed
// requirement instead of dumping the overload set:
//
//   template <class T>
//   concept PlainSignedInt = std::signed_integral<T>
//                            && !std::same_as<T, bool> && !std::same_as<T, char>;
//
//   template <PlainSignedInt T> T absValue(T x) { return x < 0 ? -x : x; }
//
// absValue("nope") then reports: constraints not satisfied -- PlainSignedInt is
// not modelled by const char* -- instead of a page of "no matching function".

int main() {
    std::cout << "absValue(-5)  -> " << absValue(-5) << "\n";
    std::cout << "absValue(-7L) -> " << absValue(-7L) << "\n";
    // absValue(3.14) or absValue("x") does not compile: the enable_if condition
    // is false, so there is no viable absValue -- with exactly the opaque error
    // concepts were designed to replace.
    return 0;
}
