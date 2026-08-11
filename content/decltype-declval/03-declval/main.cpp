#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// A type with NO default constructor -- you cannot write NoDefault{} to get one.
struct NoDefault {
    explicit NoDefault(int) {}
    std::string greet() const { return "hi"; }
};

int main() {
    // std::declval<T>() "produces" a value of type T in an UNEVALUATED context, so
    // the result type of an operation on T can be named without constructing a T.
    using GreetResult = decltype(std::declval<NoDefault>().greet());   // std::string
    static_assert(std::is_same<GreetResult, std::string>::value, "greet() returns std::string");

    // Without declval you would need an object, which NoDefault cannot default-make:
    //   decltype(NoDefault{}.greet())   // ERROR: no default constructor

    NoDefault n(42);   // a real object needs the real constructor
    std::cout << "greet() type deduced; a sample value is \"" << n.greet() << "\"\n";
    return 0;
}
