#include <functional>
#include <iostream>
#include <string>

struct Widget {
    std::string name;
    int clicks = 0;
    void click() { ++clicks; }
    const std::string& label() const { return name; }
};

int main() {
    Widget w{"OK", 0};

    // With a pointer-to-member the SECOND argument is the object, and std::invoke
    // accepts it in every natural form -- doing the right dereferencing for you.

    // 1) by reference: calls on the caller's own object, so it mutates it.
    std::invoke(&Widget::click, w);
    std::invoke(&Widget::click, w);
    std::cout << "after ref calls, clicks = " << w.clicks << "\n";   // 2

    // 2) through a pointer: same object, pointer syntax handled for you.
    Widget* pw = &w;
    std::invoke(&Widget::click, pw);
    std::cout << "after ptr call, clicks  = " << w.clicks << "\n";   // 3

    // 3) through a std::reference_wrapper: also refers to the same object.
    std::invoke(&Widget::click, std::ref(w));
    std::cout << "after ref_wrap, clicks  = " << w.clicks << "\n";   // 4

    // 4) a const member function, and reading a data member by pointer-to-member.
    std::cout << "label() = " << std::invoke(&Widget::label, w) << "\n";  // OK
    std::cout << "name    = " << std::invoke(&Widget::name, w) << "\n";   // OK

    // Passing the object BY VALUE clicks a copy, not w -- so pass a reference or
    // pointer when you mean to change the original.
    Widget copy = w;
    std::invoke(&Widget::click, copy);       // mutates the copy only
    std::cout << "w.clicks stays " << w.clicks
              << ", copy.clicks = " << copy.clicks << "\n";          // 4, 5
    return 0;
}
