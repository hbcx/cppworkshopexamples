// Anti-pattern: inverting a STABLE dependency (over-applying DIP).
//
// The bad Greeter depends on an IStringBuilder interface, with an adapter and
// injection, just to join two strings. std::string is never going to be swapped,
// so the interface, adapter, and wiring buy nothing and cost a layer of
// indirection.
//
// The fix: use std::string directly. Invert dependencies on VOLATILE details
// (I/O, services, time), not on stable, ubiquitous ones.

#include <iostream>
#include <string>

// ---------- an interface over a dependency that will never change ----------
namespace bad {

class IStringBuilder {
public:
    virtual ~IStringBuilder() {}
    virtual void append(const std::string& s) = 0;
    virtual std::string result() const = 0;
};

class StdStringBuilder : public IStringBuilder {
public:
    void append(const std::string& s) override { buffer_ += s; }
    std::string result() const override { return buffer_; }
private:
    std::string buffer_;
};

class Greeter {
public:
    explicit Greeter(IStringBuilder& builder) : builder_(builder) {}
    std::string greet(const std::string& name) {
        builder_.append("Hi, ");
        builder_.append(name);
        return builder_.result();
    }
private:
    IStringBuilder& builder_;
};

} // namespace bad

// ---------- just use the stable type directly ----------
namespace good {

class Greeter {
public:
    std::string greet(const std::string& name) { return "Hi, " + name; }
};

} // namespace good

int main() {
    bad::StdStringBuilder builder;
    bad::Greeter legacy(builder);
    std::cout << "[bad]  " << legacy.greet("Ada") << " (interface + adapter + wiring)\n";

    good::Greeter greeter;
    std::cout << "[good] " << greeter.greet("Ada") << " (just std::string)\n";
}
