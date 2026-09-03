#include <functional>
#include <iostream>
#include <memory>

// TRAP: a std::function stored for later that captured a local by reference.
// The reference dangles the moment the local's scope ends, and the stored
// function is then called on a dead object -- undefined behaviour. Because a
// std::function OUTLIVES the call that created it, the capture must outlive the
// std::function, which capture-by-reference of a local does not.
std::function<int()> makeCounterBad() {
    int count = 0;
    // return [&count] { return ++count; };   // UB: count dies at return.
    // Shown only as a comment -- running it would read a dangling reference,
    // which is undefined and could crash or differ between compilers.

    // FIX 1: capture by VALUE. The closure owns its own copy of the state.
    return [count]() mutable { return ++count; };
}

// FIX 2: when the state must be SHARED and outlive the scope, own it through a
// shared_ptr and capture that -- the state lives as long as any copy of the
// function does.
std::function<int()> makeCounterShared() {
    auto count = std::make_shared<int>(0);
    return [count] { return ++*count; };
}

int main() {
    auto c1 = makeCounterBad();
    std::cout << "by-value counter : " << c1() << " " << c1() << " " << c1() << "\n"; // 1 2 3

    auto c2 = makeCounterShared();
    std::cout << "shared counter   : " << c2() << " " << c2() << " " << c2() << "\n"; // 1 2 3

    std::cout << "\nRule: a stored std::function must not capture a local by\n"
                 "reference -- capture by value, or share ownership of the state.\n";
    return 0;
}
