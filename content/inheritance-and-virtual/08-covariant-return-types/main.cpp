#include <iostream>
#include <memory>
#include <string>

// A virtual clone() copies a polymorphic object through a base handle. Covariant
// return types let each override return its own type, so calling clone() on a Dog
// gives back a Dog*, not just an Animal*.

class Animal {
public:
    explicit Animal(std::string name) : name_(std::move(name)) {}
    virtual Animal* clone() const { return new Animal(*this); }   // returns Animal*
    virtual std::string describe() const { return "Animal " + name_; }
    virtual ~Animal() = default;
protected:
    std::string name_;
};

class Dog : public Animal {
public:
    Dog(std::string name, std::string breed)
        : Animal(std::move(name)), breed_(std::move(breed)) {}
    // Covariant: overrides Animal::clone but returns the more derived Dog*.
    Dog* clone() const override { return new Dog(*this); }
    std::string describe() const override { return "Dog " + name_ + " (" + breed_ + ")"; }
private:
    std::string breed_;
};

int main() {
    Dog rex{"Rex", "terrier"};

    // Called on a Dog, clone() yields a Dog* -- the covariant return type, no cast:
    std::unique_ptr<Dog> copy{rex.clone()};
    std::cout << copy->describe() << "\n";        // Dog Rex (terrier)

    // Called through a base handle, it still copies the real type (a Dog):
    Animal& a = rex;
    std::unique_ptr<Animal> polyCopy{a.clone()};
    std::cout << polyCopy->describe() << "\n";     // Dog Rex (terrier)
    return 0;
}
