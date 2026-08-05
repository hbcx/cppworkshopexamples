// Depend on the smallest interface that covers your need.
//
// save() needs one thing from whatever it stores: a byte representation. So it
// depends on Serializable (one method), not on the whole User class. User has
// other methods and will grow more, but a change to any of them cannot touch
// save() -- because save() never depended on them.

#include <iostream>
#include <string>

// The one capability the persistence layer needs.
class Serializable {
public:
    virtual ~Serializable() {}
    virtual std::string toBytes() const = 0;
};

// A rich domain object that ALSO happens to be serializable.
class User : public Serializable {
public:
    User(std::string name, int age) : name_(std::move(name)), age_(age) {}

    // Domain behaviour save() never sees.
    std::string greet() const { return "Hi, I am " + name_; }
    void birthday() { ++age_; }

    // The one role the persistence layer depends on.
    std::string toBytes() const override {
        return name_ + ";" + std::to_string(age_);
    }
private:
    std::string name_;
    int age_;
};

// Depends only on Serializable -- decoupled from everything else User does.
void save(const Serializable& s) {
    std::cout << "saving: " << s.toBytes() << "\n";
}

int main() {
    User u("Ada", 30);
    std::cout << u.greet() << "\n"; // User has more, but...
    save(u);                        // ...save() sees only toBytes()
    u.birthday();
    save(u);
}
