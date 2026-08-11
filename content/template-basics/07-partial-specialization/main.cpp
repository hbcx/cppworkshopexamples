#include <iostream>
#include <string>

// The primary template.
template <class T>
struct Describe {
    static std::string what() { return "a value"; }
};

// PARTIAL specialization: still parameterised (on T), but matches a FAMILY --
// here any pointer T*. It is more specialised than the primary, so a pointer
// selects it.
template <class T>
struct Describe<T*> {
    static std::string what() { return "a pointer"; }
};

// Another partial specialization: any array T[N].
template <class T, int N>
struct Describe<T[N]> {
    static std::string what() { return "an array"; }
};

int main() {
    std::cout << "int:    " << Describe<int>::what() << "\n";     // a value (primary)
    std::cout << "int*:   " << Describe<int*>::what() << "\n";    // a pointer
    std::cout << "int[5]: " << Describe<int[5]>::what() << "\n";  // an array
    return 0;
}
