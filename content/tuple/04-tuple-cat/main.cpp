#include <iostream>
#include <string>
#include <tuple>

int main() {
    // std::tuple_cat joins several tuples end to end into one, preserving every
    // element type -- handy for building a combined record from parts.
    auto name = std::make_tuple(std::string("Ada"), std::string("Lovelace"));
    auto meta = std::make_tuple(1815, true);

    auto person = std::tuple_cat(name, meta);   // tuple<string,string,int,bool>

    std::cout << std::get<0>(person) << ' ' << std::get<1>(person)
              << ", born " << std::get<2>(person)
              << ", active? " << std::boolalpha << std::get<3>(person) << '\n';

    // The result's field count is the sum of the parts, known at compile time.
    std::cout << "fields = " << std::tuple_size<decltype(person)>::value << '\n';
    return 0;
}
