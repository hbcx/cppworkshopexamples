#include <functional>
#include <iostream>

int main() {
    int a = 10;
    int b = 20;

    // std::ref wraps a reference in a copyable, assignable OBJECT -- a
    // std::reference_wrapper<int>. Unlike a real reference (int&), you can copy
    // it, reassign it, and store it in a container.
    std::reference_wrapper<int> r = std::ref(a);

    // Read or write the referent: implicit conversion to int&, or .get().
    std::cout << "r refers to a = " << r.get() << "\n";   // 10
    r.get() = 11;                                         // writes through to a
    std::cout << "a is now " << a << "\n";                // 11

    int sum = r + 100;   // implicit conversion to int& reads 11
    std::cout << "r + 100 = " << sum << "\n";             // 111

    // A reference cannot be reseated; a reference_wrapper CAN -- assigning a new
    // std::ref makes it refer to a different object.
    r = std::ref(b);
    std::cout << "r now refers to b = " << r.get() << "\n";        // 20
    r.get() = 21;
    std::cout << "b is now " << b << ", a stays " << a << "\n";    // 21, 11
    return 0;
}
