#include <iostream>
#include <string>
#include <tuple>

int main() {
    auto t = std::make_tuple(10, std::string("mid"), 2.5);

    // Anti-pattern: indexing a tuple with a RUNTIME variable. std::get takes the
    // index as a template argument -- a compile-time constant -- so there is no
    // get(i), and a loop counter will not compile. Left in a comment because it
    // is a hard error, not something that runs:
    //
    //     for (std::size_t i = 0; i < 3; ++i)
    //         std::cout << std::get<i>(t);   // error: i is not a constant expression
    //
    // The elements differ in type per position, so a runtime index could not
    // even have a single return type -- the tuple is not an array.

    // Fix: std::apply hands every element to a generic lambda, and a C++17 fold
    // expression visits each in turn -- the compile-time "loop" a tuple needs.
    std::apply([](const auto&... elems) {
        ((std::cout << elems << ' '), ...);
    }, t);
    std::cout << '\n';

    // When a runtime index is genuinely required, the data is homogeneous --
    // use std::array or std::vector, where operator[] with a runtime index fits.
    return 0;
}
