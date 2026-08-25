#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Anti-pattern: taking c_str() of a TEMPORARY string returned by str().
    // str() returns a NEW std::string by value; if you do not keep it alive, it
    // is destroyed at the end of the full expression and the pointer dangles.
    //
    //   const char* p = ss.str().c_str();   // BUG: temporary gone after this line
    //   use(p);                             // p now points at freed memory
    //
    // We do NOT dereference a dangling pointer here (that is undefined
    // behaviour); we show the safe forms instead.
    std::ostringstream ss;
    ss << "session-" << 7;

    // Fix 1: keep the string in a named variable, then use it (or its c_str()).
    std::string kept = ss.str();
    const char* p = kept.c_str();      // valid: 'kept' outlives p
    std::cout << "kept: " << p << '\n';                 // session-7
    std::cout << "length: " << std::strlen(p) << '\n';  // 9

    // Fix 2: if you only need the std::string, just use it directly.
    std::cout << "direct: " << ss.str() << '\n';        // session-7
    return 0;
}
