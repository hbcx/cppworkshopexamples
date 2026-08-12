#include <iostream>
#include <type_traits>

// std::enable_if<Cond, T>::type is T when Cond is true, and does not exist when
// Cond is false. Naming that missing ::type in a template's signature is a
// substitution failure, so enable_if turns an overload on or off. There are
// three places to put it.

// Position A: on the RETURN type. Works anywhere, but buries the real return
// type (T here) inside the enable_if.
template <class T>
typename std::enable_if<std::is_integral<T>::value, T>::type
doubleA(T x) { return x + x; }

// Position B: an extra defaulted TEMPLATE parameter. The clearest form -- the
// return type stays honest, and constructors (which have no return type) can use
// it too. Note it is a non-type parameter with a value (= 0), NOT a defaulted
// type parameter; see the redefinition anti-pattern for why that matters.
template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
T doubleB(T x) { return x + x; }

// Position C: a defaulted FUNCTION parameter. Works, but adds a phantom argument
// to the signature that shows up in error messages.
template <class T>
T doubleC(T x, typename std::enable_if<std::is_integral<T>::value, int>::type = 0) {
    return x + x;
}

int main() {
    std::cout << "A: " << doubleA(21) << "\n";
    std::cout << "B: " << doubleB(21) << "\n";
    std::cout << "C: " << doubleC(21) << "\n";
    // doubleA(3.14) would not compile: is_integral<double> is false, so the only
    // overload is disabled and there is no viable function to call.
    return 0;
}
