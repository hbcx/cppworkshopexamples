#include <iostream>
#include <string>
#include <type_traits>

// When the two branches live in ONE function and differ only by a compile-time
// condition, a pair of enable_if overloads is more code than the job needs.
// C++17 if constexpr keeps both branches in one function and compiles only the
// taken one.

// OLD: two overloads, the trait spelled out twice, the signature repeated, the
// conditions kept mutually exclusive by hand.
template <class T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
nameOld(T) { return "integer"; }
template <class T>
typename std::enable_if<!std::is_integral<T>::value, std::string>::type
nameOld(T) { return "other"; }

// NEW: one function. The discarded branch is not instantiated, so each branch may
// even use operations that would be ill-formed for the other type.
template <class T>
std::string nameNew(T) {
    if constexpr (std::is_integral<T>::value)
        return "integer";
    else
        return "other";
}

int main() {
    std::cout << "nameOld(42)   -> " << nameOld(42) << "\n";
    std::cout << "nameOld(3.14) -> " << nameOld(3.14) << "\n";
    std::cout << "nameNew(42)   -> " << nameNew(42) << "\n";
    std::cout << "nameNew(3.14) -> " << nameNew(3.14) << "\n";
    return 0;
}
