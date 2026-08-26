// Every expression is either an lvalue or an rvalue. Roughly: an lvalue has a
// name and an address you can take (a variable, a[i], *p); an rvalue is a
// temporary with no lasting identity (a literal, a function returning by value,
// a + b). The distinction drives overload resolution: an lvalue and an rvalue can
// pick different functions.
#include <iostream>
#include <string>

void inspect(const std::string&) { std::cout << "lvalue (or const)\n"; }
void inspect(std::string&&)      { std::cout << "rvalue (temporary)\n"; }

std::string make() { return "temp"; }    // returns a temporary -> rvalue

int main() {
    std::string named = "hello";

    inspect(named);            // has a name -> lvalue
    inspect(make());           // a returned temporary -> rvalue
    inspect(std::string("x")); // an explicit temporary -> rvalue
    inspect(named + "!");      // a + b yields a temporary -> rvalue
    return 0;
}
