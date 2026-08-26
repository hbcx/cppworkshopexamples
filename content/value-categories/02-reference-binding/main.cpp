// The three reference kinds bind to different things:
//   T&        -- only a modifiable lvalue
//   const T&  -- anything (lvalue, const, or rvalue): the universal read-only bind
//   T&&       -- only an rvalue (a temporary or the result of std::move)
// With all three overloads present, the compiler picks the most specific.
#include <iostream>
#include <string>
#include <utility>

void f(std::string&)       { std::cout << "T&        (modifiable lvalue)\n"; }
void f(const std::string&) { std::cout << "const T&  (const or fallback)\n"; }
void f(std::string&&)      { std::cout << "T&&       (rvalue)\n"; }

int main() {
    std::string s = "x";
    const std::string cs = "y";

    f(s);               // modifiable lvalue -> T&
    f(cs);              // const lvalue      -> const T&
    f(std::string());   // temporary rvalue  -> T&&
    f(std::move(s));    // xvalue (an rvalue) -> T&&
    return 0;
}
