#include <iostream>
#include <string>
#include <type_traits>

// conjunction/disjunction/negation combine trait predicates like &&, ||, !, and
// they short-circuit -- a later trait is not evaluated once the answer is known.
template <class T>
constexpr bool is_signed_integer_v =
    std::conjunction_v<std::is_integral<T>, std::is_signed<T>>;

template <class T>
constexpr bool is_number_or_pointer_v =
    std::disjunction_v<std::is_arithmetic<T>, std::is_pointer<T>>;

int addOne(int x) { return x + 1; }

int main() {
    std::cout << std::boolalpha;
    std::cout << "signed_integer<int>      = " << is_signed_integer_v<int> << "\n";        // true
    std::cout << "signed_integer<unsigned> = " << is_signed_integer_v<unsigned> << "\n";   // false
    std::cout << "number_or_pointer<int*>  = " << is_number_or_pointer_v<int*> << "\n";     // true

    static_assert(std::negation_v<std::is_integral<double>>);   // !is_integral<double>

    // is_invocable asks whether a call is valid; invoke_result gives its return type.
    static_assert(std::is_invocable_v<decltype(addOne), int>);
    static_assert(!std::is_invocable_v<decltype(addOne), std::string>);
    static_assert(std::is_same_v<std::invoke_result_t<decltype(addOne), int>, int>);

    std::cout << "logical traits and invocable hold\n";
    return 0;
}
