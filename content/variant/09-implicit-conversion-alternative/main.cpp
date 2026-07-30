#include <iostream>
#include <utility>   // std::in_place_type, std::in_place_index
#include <variant>

int main() {
    std::cout << std::boolalpha;

    // Anti-pattern: relying on implicit conversion to choose the alternative. A
    // value whose type is not itself an alternative converts to whichever one it
    // can -- not always the one you picture. A float has no exact alternative
    // here, so it becomes the double (float->int is narrowing and excluded):
    std::variant<int, double> v = 3.0f;
    std::cout << "float stored as double? " << (v.index() == 1) << '\n';   // true

    // Worse, when a value converts EQUALLY well to two alternatives the
    // assignment is ambiguous and does NOT compile. An int literal fits both
    // long and long long, so this line is a hard error (shown, not run):
    //
    //     std::variant<long, long long> bad = 0;   // error: ambiguous conversion
    //
    // (variant<std::string, bool> = "literal" once picked bool; C++17's P0608
    //  fixed that specific case, but overlapping conversions still bite.)

    // Fix: name the alternative -- by type or by index -- so there is no
    // conversion to resolve.
    std::variant<long, long long> a(std::in_place_type<long long>, 0);
    std::cout << "in_place_type<long long> -> index 1? " << (a.index() == 1) << '\n';
    std::variant<long, long long> b(std::in_place_index<0>, 0);
    std::cout << "in_place_index<0> (long)  -> index 0? " << (b.index() == 0) << '\n';
    return 0;
}
