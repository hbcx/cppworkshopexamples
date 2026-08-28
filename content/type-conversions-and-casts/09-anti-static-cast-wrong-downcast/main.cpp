#include <iostream>

struct Animal { virtual ~Animal() = default; };
struct Dog : Animal { int legs = 4; };
struct Cat : Animal {};

int main() {
    Cat cat;
    Animal* a = &cat;      // really a Cat, seen as an Animal

    // BAD: static_cast down to Dog* compiles and yields a non-null pointer, but the
    // object is a Cat -- the pointer is invalid, and reading dog->legs would be
    // undefined behavior. We do NOT dereference it.
    Dog* wrong = static_cast<Dog*>(a);
    std::cout << "static_cast gave a non-null pointer? " << (wrong != nullptr)
              << "  (but using it is undefined behavior)\n";        // 1

    // GOOD: dynamic_cast checks the real type and reports the mismatch as nullptr.
    Dog* checked = dynamic_cast<Dog*>(a);
    std::cout << "dynamic_cast<Dog*> on a Cat: "
              << (checked == nullptr ? "nullptr (correctly rejected)" : "Dog") << "\n";

    // And on a real Dog, dynamic_cast succeeds and is safe to use:
    Dog dog;
    Animal* a2 = &dog;
    if (Dog* d = dynamic_cast<Dog*>(a2))
        std::cout << "real Dog has " << d->legs << " legs\n";       // 4
    return 0;
}
