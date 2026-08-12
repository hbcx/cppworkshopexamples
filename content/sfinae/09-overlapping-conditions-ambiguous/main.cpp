#include <iostream>
#include <type_traits>

// A pair of enable_if overloads only works if their conditions can never BOTH be
// true. When the conditions overlap, a call that satisfies both has two equally
// good candidates, and the compiler reports an ambiguous call.

// BAD (shown in a comment -- ambiguous for T = int):
//
//   template <class T>
//   typename std::enable_if<std::is_integral<T>::value, const char*>::type
//   h(T) { return "integral"; }
//   template <class T>
//   typename std::enable_if<(sizeof(T) <= 4), const char*>::type
//   h(T) { return "small"; }
//
// For int BOTH conditions hold (int is integral AND sizeof(int) <= 4), both
// overloads are viable with an identical parameter, and h(0) does not compile:
// "call to h is ambiguous".

// GOOD: make the second condition EXCLUDE the first, so the two partition the
// types instead of overlapping.
template <class T>
typename std::enable_if<std::is_integral<T>::value && (sizeof(T) <= 4), const char*>::type
h(T) { return "small integer"; }

template <class T>
typename std::enable_if<std::is_integral<T>::value && (sizeof(T) > 4), const char*>::type
h(T) { return "large integer"; }

int main() {
    std::cout << "h(42)   -> " << h(42) << "\n";     // int, 4 bytes
    std::cout << "h(42LL) -> " << h(42LL) << "\n";   // long long, 8 bytes
    return 0;
}
