// Anti-pattern: returning a reference to a local. A function's local objects are
// destroyed when it returns, so a reference (or pointer) to one dangles -- using
// it is undefined behaviour. The same trap hides in decltype(auto) and in an
// auto&& that captures a local. Return by value instead; move and copy elision
// make it cheap.
#include <iostream>
#include <string>

// WRONG (described, not run -- it is undefined behaviour):
//
//   const std::string& bad() {
//       std::string local = "temporary";
//       return local;                 // local dies here -> the returned ref dangles
//   }
//   const std::string& r = bad();     // r refers to destroyed memory; reading it is UB
//
// RIGHT: return by value. Copy elision or a move makes this as cheap as it looks.
std::string good() {
    std::string local = "safe result";
    return local;                      // elided or moved out -- a valid value
}

int main() {
    std::string r = good();
    std::cout << "value returned safely: \"" << r << "\"\n";
    return 0;
}
