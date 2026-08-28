#include <iostream>

int main() {
    // Here (int) is really a static_cast -- a meaningful numeric conversion:
    double d = 3.9;
    int a = (int)d;
    std::cout << "(int)3.9 = " << a << "\n";                 // 3

    // Here (char*) on a const char* is really a const_cast -- it strips const.
    // Same syntax, completely different operation, and nothing at the call site
    // tells you which one happened:
    const char* msg = "hi";
    char* writable = (char*)msg;
    std::cout << "(char*) stripped const, non-null? " << (writable != nullptr) << "\n";  // 1

    // The named casts make each intent explicit and let the compiler object:
    //   static_cast<char*>(msg)   // ERROR: static_cast cannot cast away const
    //   const_cast<char*>(msg)    // OK, and it SAYS it is about const
    int n = static_cast<int>(d);   // clearly a numeric conversion
    std::cout << "static_cast<int>(3.9) = " << n << "\n";    // 3
    return 0;
}
