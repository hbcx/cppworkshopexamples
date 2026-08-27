#include <iostream>
#include <string>
#include <utility>

// A data member declared const gets its value once, in the constructor's member
// initializer list, and never changes again. That has a consequence people miss:
// a class with a const member cannot be ASSIGNED (there is no way to reassign the
// const member), though it can still be COPIED or MOVED into a NEW object.

class Sensor {
public:
    Sensor(std::string name, int id)
        : name_(std::move(name)), id_(id) {   // const member set HERE, in the list
        // id_ = id;   // error: cannot assign to a const member in the body
    }

    const std::string& name() const { return name_; }
    int id() const { return id_; }

    void rename(std::string name) { name_ = std::move(name); }  // OK: name_ not const

private:
    std::string name_;
    const int   id_;     // fixed for the object's whole life
};

int main() {
    Sensor a("intake", 1);
    Sensor b("exhaust", 2);
    std::cout << a.name() << " id=" << a.id() << "\n";
    std::cout << b.name() << " id=" << b.id() << "\n";

    a.rename("intake-A");        // a non-const member can still change
    std::cout << "renamed: " << a.name() << " id=" << a.id() << "\n";

    // Copy CONSTRUCTION is fine: a new object, its const id_ initialized once.
    Sensor c = a;
    std::cout << "copy: " << c.name() << " id=" << c.id() << "\n";

    // a = b;   // error: copy assignment is implicitly deleted -- you cannot
    //          // reassign the const member id_. That is the price of a const
    //          // member, and the reason to use one only when it truly never
    //          // changes and the object never needs to be reassigned.
    std::cout << "(a = b would not compile: the const member blocks assignment)\n";
    return 0;
}
