// Anti-pattern: assuming every T&& is a forwarding reference. It is one ONLY when
// T is a template parameter DEDUCED directly from that argument. These look
// similar but are plain RVALUE references that reject lvalues:
//   - std::vector<T>&& v   (T is deduced, but the parameter is not just T&&)
//   - const T&& x          (const disqualifies it)
//   - a class-template member taking the CLASS's T&& (already fixed, not deduced)
// Only a true forwarding reference needs std::forward and accepts lvalues.
#include <iostream>
#include <vector>
#include <utility>

// A real forwarding reference: binds an lvalue and an rvalue.
template <typename T>
void forwarding(T&&) { std::cout << "forwarding ref: bound (lvalue or rvalue)\n"; }

// NOT forwarding: std::vector<T>&& is a plain rvalue reference to a vector<T>.
template <typename T>
void not_forwarding(std::vector<T>&&) { std::cout << "plain rvalue ref: rvalue only\n"; }

int main() {
    std::vector<int> v = {1, 2, 3};

    forwarding(v);                            // lvalue accepted
    forwarding(std::move(v));                 // rvalue accepted too

    not_forwarding(std::vector<int>{4, 5});   // rvalue: OK
    // not_forwarding(v);                     // would NOT compile: rejects an lvalue
    return 0;
}
