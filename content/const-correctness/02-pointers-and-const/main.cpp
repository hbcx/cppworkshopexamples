#include <iostream>

// With pointers there are two independent things you can freeze: the pointee
// (what it points AT) and the pointer (where it points). Read the declaration
// right to left: "const int*" is a pointer to const int; "int* const" is a
// const pointer to int.

int main() {
    int a = 1;
    int b = 2;

    // Pointer to const int: you may repoint it, but not write through it.
    const int* toConst = &a;
    toConst = &b;               // OK: repoint
    // *toConst = 9;            // error: cannot write through pointer to const
    std::cout << "pointer-to-const now reads: " << *toConst << "\n";   // 2

    // Const pointer to int: you may write through it, but not repoint it.
    int* const constPtr = &a;
    *constPtr = 9;              // OK: write through
    // constPtr = &b;          // error: cannot repoint a const pointer
    std::cout << "const-pointer wrote a = " << a << "\n";              // 9

    // Const pointer to const int: neither is allowed.
    const int* const both = &b;
    // *both = 0;  both = &a;   // both errors
    std::cout << "const-pointer-to-const reads: " << *both << "\n";    // 2

    // A reference to const is the "pointer to const" case, and a reference can
    // never be repointed anyway.
    const int& r = a;
    std::cout << "reference-to-const reads a = " << r << "\n";         // 9
    return 0;
}
