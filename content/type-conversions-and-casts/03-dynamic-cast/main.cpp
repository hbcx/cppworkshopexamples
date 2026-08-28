#include <iostream>
#include <typeinfo>   // std::bad_cast

struct Animal {
    virtual ~Animal() = default;   // polymorphic: dynamic_cast needs a virtual
};
struct Dog : Animal {
    void fetch() const { std::cout << "fetch!\n"; }
};
struct Cat : Animal {};

int main() {
    Dog dog;
    Cat cat;
    Animal* a1 = &dog;
    Animal* a2 = &cat;

    // Pointer form: succeeds -> valid Dog*, fails -> nullptr. Branch on the result.
    if (Dog* d = dynamic_cast<Dog*>(a1)) {
        std::cout << "a1 is a Dog: ";
        d->fetch();                                    // fetch!
    }
    Dog* notDog = dynamic_cast<Dog*>(a2);
    std::cout << "a2 as Dog* is null? " << (notDog == nullptr) << "\n";   // 1

    // Reference form: no null to return, so a bad cast throws std::bad_cast.
    try {
        Dog& d = dynamic_cast<Dog&>(*a2);   // *a2 is a Cat -> throws
        (void)d;
    } catch (const std::bad_cast&) {
        std::cout << "reference cast threw bad_cast\n";                    // printed
    }
    return 0;
}
