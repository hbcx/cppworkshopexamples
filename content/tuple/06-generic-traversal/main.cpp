#include <iostream>
#include <string>
#include <tuple>

int main() {
    // A tuple is heterogeneous, so you cannot loop over it with a normal for.
    // std::apply hands all elements to a callable at once; a C++17 fold
    // expression over the parameter pack then processes each in turn -- the
    // standard way to "iterate" a tuple generically.
    auto t = std::make_tuple(42, std::string("hi"), 3.14, 'z');

    int count = 0;
    std::apply(
        [&](const auto&... elems) {
            // Fold the stream insertion over every element, whatever its type.
            ((std::cout << "[" << elems << "]"), ...);
            count = static_cast<int>(sizeof...(elems));
        },
        t);
    std::cout << "\nvisited " << count << " elements\n";

    // tuple_size exposes the arity at compile time, which is what makes this
    // kind of generic code possible in the first place.
    std::cout << "size = " << std::tuple_size<decltype(t)>::value << '\n';
    return 0;
}
