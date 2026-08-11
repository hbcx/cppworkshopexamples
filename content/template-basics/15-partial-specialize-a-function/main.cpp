#include <iostream>

// BAD (described, does not compile): function templates cannot be partially
// specialized. This syntax is ill-formed:
//   template <class T> void handle(T);           // primary
//   template <class T> void handle<T*>(T* p);    // NOT allowed for functions

// GOOD: use OVERLOADING. Two overloads, one for T and one for T*, and overload
// resolution prefers the more specific one.
template <class T>
void handle(T)  { std::cout << "value\n"; }
template <class T>
void handle(T*) { std::cout << "pointer\n"; }

int main() {
    int x = 5;
    handle(x);    // value   (T = int)
    handle(&x);   // pointer (the T* overload is more specialized)
    return 0;
}
