#include <iostream>
#include <string>

// Animal is the base. Dog is-an Animal, so it inherits publicly and adds its own
// data. The derived object contains an Animal subobject, initialized first.

class Animal {
public:
    explicit Animal(std::string name) : name_(std::move(name)) {}

    const std::string& name() const { return name_; }   // reused by Dog as-is

protected:
    std::string name_;   // visible to Dog, not to outside code
};

class Dog : public Animal {
public:
    Dog(std::string name, std::string breed)
        : Animal(std::move(name)),          // initialize the base subobject first
          breed_(std::move(breed)) {}

    // Uses the protected base member directly.
    std::string describe() const { return name_ + " is a " + breed_; }

private:
    std::string breed_;
};

int main() {
    Dog d{"Rex", "terrier"};

    std::cout << d.describe() << "\n";        // Rex is a terrier
    std::cout << "name: " << d.name() << "\n"; // Rex   (inherited base method)
    return 0;
}
