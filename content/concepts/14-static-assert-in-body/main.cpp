#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

// BAD: the check lives in the BODY. handleOld is ALWAYS the selected function for
// any T, then hard-errors inside if T is wrong -- so you cannot add a fallback
// overload for other types, and the error comes after selection.
template <class T>
void handleOld(const T& x) {
    static_assert(std::is_integral_v<T>, "handleOld needs an integral type");
    std::cout << "old integer: " << x << "\n";
}

// GOOD: the constraint is in the SIGNATURE, so it takes part in overload
// resolution. The constrained overload is viable only for integers, and a second
// overload cleanly handles everything else.
template <std::integral T>
void handleNew(const T& x) { std::cout << "new integer: " << x << "\n"; }
template <class T>
void handleNew(const T& x) { std::cout << "new other:   " << x << "\n"; }

int main() {
    handleOld(42);                    // ok
    handleNew(42);                    // integer overload
    handleNew(std::string("hi"));     // other overload -- impossible with static_assert

    // handleOld(std::string("hi"));  // does NOT fall back: it is still selected,
    //   then the static_assert fires -- a hard error, with no way to route strings
    //   elsewhere. A body check cannot disable an overload; a constraint can.
    std::cout << "done\n";
}
