#include <iostream>

// A function template and a plain OVERLOAD.
template <class T>
void f(T)    { std::cout << "template\n"; }
void f(int*) { std::cout << "overload(int*)\n"; }

// A full SPECIALIZATION of the template for int*. It looks like it should be
// picked for an int*, but specializations do NOT take part in overload
// resolution -- the compiler picks the best overload FIRST.
template <>
void f<int*>(int*) { std::cout << "specialization(int*)\n"; }

int main() {
    int x = 0;
    f(&x);   // prints "overload(int*)", NOT "specialization(int*)":
             // overload resolution runs first and the plain overload wins, so the
             // specialization is never considered. Prefer overloading here.
    return 0;
}
