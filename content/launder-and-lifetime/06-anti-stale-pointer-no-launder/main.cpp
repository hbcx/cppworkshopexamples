// Anti-pattern: after reusing storage, reading through the pointer you held
// before the reuse.
//
// A reference member is fixed for an object's life, so the compiler may keep
// assuming the old binding when you read the old pointer -- undefined, and it
// often prints right until the optimizer breaks it. Use the new-expression's
// return value, or std::launder the old pointer.

#include <iostream>
#include <new>

struct Binder {
    int& ref;                       // reference member: fixed for this object
    explicit Binder(int& r) : ref(r) {}
};

int main() {
    int first = 10, second = 20;
    alignas(Binder) unsigned char storage[sizeof(Binder)];

    Binder* p = new (storage) Binder(first);
    std::cout << "first binding: " << p->ref << '\n';   // 10

    // Reuse the storage for a Binder bound to a different int.
    Binder* fresh = new (storage) Binder(second);

    // BAD (undefined -- described, not run):
    //   std::cout << p->ref;   // may still read `first` (10): the compiler
    //                          // assumes the reference member did not change.

    // GOOD 1: use the pointer the new-expression returned.
    std::cout << "via returned pointer:  " << fresh->ref << '\n';   // 20
    // GOOD 2: launder the old pointer to reach the new object.
    std::cout << "via laundered pointer: " << std::launder(p)->ref << '\n';   // 20

    std::launder(p)->~Binder();
}
