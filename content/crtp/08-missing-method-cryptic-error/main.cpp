// Anti-pattern: a CRTP base's required interface is implicit, so a missing
// method gives a cryptic error deep in the base template.
//
// The base calls draw() on the derived type, but that requirement is written
// nowhere. A derived type that forgets draw() fails INSIDE the base with an
// unhelpful message -- not a clear "you forgot draw()".
//
// The fix: document the required methods and add a static_assert (or a C++20
// concept) that fails with a message naming the real problem.

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// A tiny C++11 detector for "does Derived have draw()?" so the base can assert
// the contract and report it clearly.
template <class T>
class HasDraw {
    template <class U>
    static auto test(int) -> decltype(std::declval<const U&>().draw(), std::true_type());
    template <class>
    static std::false_type test(...);
public:
    static const bool value = decltype(test<T>(0))::value;
};

// ---------- required interface documented AND enforced ----------
// Contract: Derived must provide `std::string draw() const`.
template <class Derived>
class Shape {
public:
    void render() const {
        static_assert(HasDraw<Derived>::value,
                      "CRTP contract: Derived must provide a draw() method");
        std::cout << static_cast<const Derived&>(*this).draw() << "\n";
    }
};

struct Circle : Shape<Circle> {
    std::string draw() const { return "(circle)"; } // satisfies the contract
};

// A type that FORGETS draw():
//     struct Broken : Shape<Broken> {};
//     Broken{}.render();
// Without the static_assert this fails deep inside render() with a "no member
// named 'draw'" error pointing at the base. WITH the static_assert it fails with
// the clear message above. Left un-instantiated so the example builds.

int main() {
    Circle c;
    c.render();
    std::cout << "(a type missing draw() is rejected by static_assert, not by a "
                 "wall of template errors)\n";
}
