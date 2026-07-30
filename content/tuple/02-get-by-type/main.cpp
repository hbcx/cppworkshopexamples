#include <iostream>
#include <string>
#include <tuple>

int main() {
    // When each type in the tuple is unique, C++14 lets you fetch an element by
    // its TYPE instead of counting indices -- clearer and refactor-proof.
    std::tuple<int, std::string, double> item(7, "bolt", 0.25);

    std::cout << "id    = " << std::get<int>(item) << '\n';
    std::cout << "name  = " << std::get<std::string>(item) << '\n';
    std::cout << "price = " << std::get<double>(item) << '\n';

    // get<T> also assigns through, like get<I>.
    std::get<std::string>(item) = "hex bolt";
    std::cout << "renamed to " << std::get<std::string>(item) << '\n';

    // The type must be UNIQUE in the tuple. With two elements of the same type
    // the request is ambiguous and does NOT compile -- you must index instead:
    //     std::tuple<int, int> point(3, 4);
    //     std::get<int>(point);   // error: which int? use get<0> / get<1>
    return 0;
}
