#include <functional>
#include <iostream>
#include <string>

int main() {
    // std::hash<T> is a function object that maps a value to a std::size_t. The
    // standard provides specializations for the built-in and common library types.
    std::hash<int> hInt;
    std::hash<std::string> hStr;

    std::cout << "hash(42)      = " << hInt(42) << "\n";
    std::cout << "hash(\"hello\") = " << hStr("hello") << "\n";
    std::cout << "hash(\"world\") = " << hStr("world") << "\n";

    // Same input, same hash within one run; different inputs almost always differ.
    std::cout << std::boolalpha
              << "hash(42) reproducible in-run? " << (hInt(42) == std::hash<int>{}(42)) << "\n"; // true

    // Pointers have a std::hash too (it hashes the address).
    int x = 0;
    std::hash<int*> hPtr;
    std::cout << "hash(&x) nonzero? " << (hPtr(&x) != 0) << "\n";   // true

    // NOTE: there is NO std::hash for std::pair, std::tuple, or the containers --
    // std::hash<std::pair<int,int>>{}(...) would not compile. For a multi-field
    // key you combine the members' hashes yourself; see the combining example.
    return 0;
}
