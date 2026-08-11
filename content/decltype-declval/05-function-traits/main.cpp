#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>

// A trait that decomposes a function TYPE into its return type, parameter types
// and count -- the only way to reach a parameter's type, which decltype cannot.
template <class F>
struct fn_traits;

template <class R, class... Args>
struct fn_traits<R(Args...)> {
    using result = R;
    static constexpr std::size_t arity = sizeof...(Args);
    template <std::size_t I>
    using arg = typename std::tuple_element<I, std::tuple<Args...>>::type;
};

// A member-function variant: strip the class and reuse the shape above.
template <class R, class C, class... Args>
struct fn_traits<R(C::*)(Args...) const> : fn_traits<R(Args...)> {};

int free_fn(double, const std::string&, int);   // declared only; decltype never calls it

struct Service {
    bool handle(int, char) const { return true; }
};

int main() {
    using FT = fn_traits<decltype(free_fn)>;
    static_assert(std::is_same<FT::result, int>::value, "free_fn returns int");
    static_assert(FT::arity == 3, "free_fn has three parameters");
    static_assert(std::is_same<FT::arg<0>, double>::value, "arg 0 is double");
    static_assert(std::is_same<FT::arg<1>, const std::string&>::value, "arg 1 is const string&");

    using MT = fn_traits<decltype(&Service::handle)>;
    static_assert(std::is_same<MT::result, bool>::value, "handle returns bool");
    static_assert(MT::arity == 2, "handle has two parameters");
    static_assert(std::is_same<MT::arg<1>, char>::value, "handle arg 1 is char");

    std::cout << "free_fn arity=" << FT::arity
              << ", Service::handle arity=" << MT::arity << "\n";
    return 0;
}
