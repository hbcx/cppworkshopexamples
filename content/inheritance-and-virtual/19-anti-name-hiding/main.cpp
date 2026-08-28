#include <iostream>

// Anti-pattern: a derived function hides ALL base overloads of that name. The base
// has print(int) and print(const char*); the derived adds its own print(int) and
// unintentionally hides the string one too.

struct Base {
    void print(int n) const        { std::cout << "base int " << n << "\n"; }
    void print(const char* s) const { std::cout << "base str " << s << "\n"; }
};

struct Bad : Base {
    // Hides BOTH Base::print overloads. bad.print("hello") does NOT compile,
    // because the const char* version is no longer visible here.
    void print(int n) const { std::cout << "bad int " << n * 2 << "\n"; }
};

struct Good : Base {
    using Base::print;                 // bring every base print back into scope
    void print(int n) const { std::cout << "good int " << n * 2 << "\n"; }
};

int main() {
    Bad bad;
    bad.print(5);            // bad int 10
    // bad.print("hello");   // would NOT compile: base string overload is hidden

    Good good;
    good.print(5);           // good int 10   (derived overload)
    good.print("hello");     // base str hello (restored by the using-declaration)
    return 0;
}
