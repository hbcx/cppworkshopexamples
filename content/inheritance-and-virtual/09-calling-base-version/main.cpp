#include <iostream>
#include <string>

// Manager::print extends Person::print rather than replacing it: it calls the base
// version for the shared fields, then adds its own.

class Person {
public:
    explicit Person(std::string name) : name_(std::move(name)) {}
    virtual void print() const {
        std::cout << "name: " << name_ << "\n";
    }
    virtual ~Person() = default;
protected:
    std::string name_;
};

class Manager : public Person {
public:
    Manager(std::string name, int reports)
        : Person(std::move(name)), reports_(reports) {}

    void print() const override {
        Person::print();                       // run the base version first...
        std::cout << "reports: " << reports_ << "\n";   // ...then add our part
    }
private:
    int reports_;
};

int main() {
    Manager m{"Ada", 5};
    m.print();
    // name: Ada
    // reports: 5
    return 0;
}
