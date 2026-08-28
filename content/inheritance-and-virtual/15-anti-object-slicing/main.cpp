#include <iostream>

// Anti-pattern: slicing. Copying a Derived into a Base value throws away the
// derived part. The virtual call then runs Base's version, because the copy is a
// Base.

class Animal {
public:
    virtual const char* speak() const { return "..."; }
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    const char* speak() const override { return "Woof"; }
};

// BAD: takes Animal BY VALUE. Any Dog passed here is sliced on the way in.
void byValue(Animal a) { std::cout << "by value: " << a.speak() << "\n"; }

// GOOD: takes a reference. The real object -- and its override -- is preserved.
void byRef(const Animal& a) { std::cout << "by ref:   " << a.speak() << "\n"; }

int main() {
    Dog dog;

    byValue(dog);   // by value: ...     (sliced to Animal, Dog::speak lost)
    byRef(dog);     // by ref:   Woof    (still a Dog)

    // Direct slicing on assignment:
    Animal a = dog;               // copies only the Animal subobject
    std::cout << "sliced copy: " << a.speak() << "\n";   // ...
    return 0;
}
