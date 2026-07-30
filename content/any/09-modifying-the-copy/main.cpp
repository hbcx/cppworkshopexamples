#include <any>
#include <iostream>
#include <string>

int main() {
    std::any a = std::string("hello");

    // Anti-pattern: any_cast<T>(a) returns a COPY. Mutating that copy (here a
    // std::string temporary) leaves the value inside the any untouched.
    std::any_cast<std::string>(a) += " world";   // modifies a throwaway copy
    std::cout << "[buggy]   stored = " << std::any_cast<std::string>(a) << '\n';   // hello

    // Fix 1: cast to a REFERENCE to reach the stored object.
    std::any_cast<std::string&>(a) += " world";
    std::cout << "[correct] stored = " << std::any_cast<std::string>(a) << '\n';   // hello world

    // Fix 2: the pointer form yields a pointer to the stored object.
    if (std::string* p = std::any_cast<std::string>(&a)) {
        *p += "!";
    }
    std::cout << "[correct] stored = " << std::any_cast<std::string>(a) << '\n';   // hello world!
    return 0;
}
