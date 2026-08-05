// The CRTP mechanic: a base templated on its own derived class.
//
//   struct Dog : Animal<Dog> { ... };
//
// Because Animal<Derived> knows the exact derived type, it can static_cast
// itself to Derived and call the derived method -- resolved at COMPILE time, no
// virtual and no vtable. speak() lives once in the base; sound() is per type.

#include <iostream>
#include <string>

template <class Derived>
class Animal {
public:
    // The shared algorithm, calling into the derived type statically.
    void speak() const {
        std::cout << self().name() << " says " << self().sound() << "\n";
    }
private:
    // Recover the concrete type. Safe because Derived derives from Animal<Derived>.
    const Derived& self() const { return static_cast<const Derived&>(*this); }
};

struct Dog : Animal<Dog> {
    std::string name() const { return "dog"; }
    std::string sound() const { return "woof"; }
};

struct Cat : Animal<Cat> {
    std::string name() const { return "cat"; }
    std::string sound() const { return "meow"; }
};

int main() {
    Dog dog;
    Cat cat;
    dog.speak(); // Animal<Dog>::speak -> Dog::sound, bound at compile time
    cat.speak(); // Animal<Cat>::speak -> Cat::sound
}
