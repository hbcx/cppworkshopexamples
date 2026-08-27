#include <iostream>
#include <string>

// C++ has several syntaxes that all "initialize" an object. This example names
// them and shows each one producing a value, so the later examples can refer to
// them by name.
int main() {
    // Default initialization: no initializer. For a class type it runs the
    // default constructor; a std::string becomes the empty string.
    std::string a;

    // Copy initialization: "= value". The right-hand side initializes the
    // object; for class types only non-explicit constructors are considered.
    std::string b = "copy";

    // Direct initialization: "(args)". Calls a matching constructor directly,
    // and may use explicit constructors too.
    std::string c(5, '*');            // five stars

    // List (brace) initialization: "{args}". The modern default. It forbids
    // narrowing and, for containers, prefers an initializer_list constructor.
    std::string d{"list"};

    // Copy-list initialization: "= {args}". Like list init but the "=" form, so
    // explicit constructors are not considered.
    std::string e = {"copy-list"};

    std::cout << "default (empty):  \"" << a << "\"\n";
    std::cout << "copy:             " << b << "\n";
    std::cout << "direct (5, '*'):  " << c << "\n";
    std::cout << "list:             " << d << "\n";
    std::cout << "copy-list:        " << e << "\n";

    // Built-in types have the same forms. int{} is value-initialized to 0.
    int n = 42;      // copy
    int m(7);        // direct
    int p{3};        // list
    int z{};         // value-initialized -> 0
    std::cout << "ints: " << n << " " << m << " " << p << " " << z << "\n";
    return 0;
}
