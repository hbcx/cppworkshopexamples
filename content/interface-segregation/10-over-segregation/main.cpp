// Anti-pattern: over-segregation (the opposite failure).
//
// Split too far and every field is its own interface: HasName, HasEmail,
// HasPhone, HasAddress. Contact inherits four bases, and printCard -- which
// needs all four to render one card -- must take four references to one object.
// The fields always travel together, so they are one role, not four interfaces.
//
// The fix groups the cohesive accessors into a single ContactInfo.

#include <iostream>
#include <string>

// ---------- one method per interface, split past any client's need ----------
namespace bad {

class HasName    { public: virtual ~HasName() {}    virtual std::string name() const = 0; };
class HasEmail   { public: virtual ~HasEmail() {}   virtual std::string email() const = 0; };
class HasPhone   { public: virtual ~HasPhone() {}   virtual std::string phone() const = 0; };
class HasAddress { public: virtual ~HasAddress() {} virtual std::string address() const = 0; };

class Contact : public HasName, public HasEmail, public HasPhone, public HasAddress {
public:
    std::string name() const override { return "Ada"; }
    std::string email() const override { return "ada@example.com"; }
    std::string phone() const override { return "555-0100"; }
    std::string address() const override { return "1 Analytical Way"; }
};

// Four references to reassemble one contact.
void printCard(const HasName& n, const HasEmail& e, const HasPhone& p, const HasAddress& a) {
    std::cout << "[bad] " << n.name() << " | " << e.email()
              << " | " << p.phone() << " | " << a.address() << "\n";
}

} // namespace bad

// ---------- the cohesive fields are one role ----------
namespace good {

class ContactInfo {
public:
    virtual ~ContactInfo() {}
    virtual std::string name() const = 0;
    virtual std::string email() const = 0;
    virtual std::string phone() const = 0;
    virtual std::string address() const = 0;
};

class Contact : public ContactInfo {
public:
    std::string name() const override { return "Ada"; }
    std::string email() const override { return "ada@example.com"; }
    std::string phone() const override { return "555-0100"; }
    std::string address() const override { return "1 Analytical Way"; }
};

// One reference: the fields are always used together.
void printCard(const ContactInfo& c) {
    std::cout << "[good] " << c.name() << " | " << c.email()
              << " | " << c.phone() << " | " << c.address() << "\n";
}

} // namespace good

int main() {
    bad::Contact bc;
    bad::printCard(bc, bc, bc, bc); // one object, four references

    good::Contact gc;
    good::printCard(gc);            // one object, one reference
}
