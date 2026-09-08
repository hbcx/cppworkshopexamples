#include <generator>
#include <iostream>
#include <string>
#include <vector>

// A reference element type: std::generator<std::string&> yields references to the
// actual objects instead of copies. The consumer sees -- and here modifies -- the
// real container elements, and no string is copied on the way out.
std::generator<std::string&> each(std::vector<std::string>& v) {
    for (std::string& s : v)
        co_yield s;
}

int main() {
    std::vector<std::string> names{"ann", "bob", "cy"};

    for (std::string& s : each(names))
        s += "!";                       // modify through the yielded reference

    std::cout << "after modifying through the generator:";
    for (const std::string& s : names)
        std::cout << " " << s;
    std::cout << "\n";
    return 0;
}
