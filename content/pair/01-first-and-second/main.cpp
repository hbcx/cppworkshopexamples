#include <iostream>
#include <string>
#include <utility>

int main() {
    // A pair bundles two values of possibly different types into one object.
    std::pair<int, std::string> entry(42, "answer");

    // The two members are plain public data members: .first and .second.
    std::cout << "first  = " << entry.first << '\n';
    std::cout << "second = " << entry.second << '\n';

    // They are mutable like any member.
    entry.first = 7;
    entry.second += "!";
    std::cout << "after edit: " << entry.first << " / " << entry.second << '\n';

    // A default-constructed pair value-initializes both members: 0 and "".
    std::pair<int, std::string> empty;
    std::cout << "default first  = " << empty.first << '\n';
    std::cout << "default second = \"" << empty.second << "\"\n";

    // Brace initialization sets both members at once (C++11).
    std::pair<double, char> p{3.5, 'x'};
    std::cout << "braced = " << p.first << " / " << p.second << '\n';
    return 0;
}
