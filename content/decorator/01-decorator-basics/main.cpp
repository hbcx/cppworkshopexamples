#include <iostream>
#include <string>
#include <utility>

// Component: the interface shared by the concrete object AND every decorator.
// A client talks only to this, and cannot tell a decorated object from a plain
// one -- that transparency is what makes Decorator composable.
struct Greeting {
    virtual std::string text() const = 0;
    virtual ~Greeting() = default;
};

// Concrete component: the thing being decorated. It knows nothing of decorators.
class PlainGreeting : public Greeting {
public:
    explicit PlainGreeting(std::string name) : name_(std::move(name)) {}
    std::string text() const override { return "Hello, " + name_; }
private:
    std::string name_;
};

// Decorator: implements the SAME interface, HOLDS a Greeting, forwards the call
// and adds behaviour around the result. This is the whole pattern -- and the
// point where it differs from Adapter: Adapter changes the interface to fit a
// client, Decorator keeps the interface identical and only grows the behaviour.
class Excited : public Greeting {
public:
    explicit Excited(const Greeting& inner) : inner_(inner) {}
    std::string text() const override { return inner_.text() + "!"; }
private:
    const Greeting& inner_;
};

// Client: accepts any Greeting, unaware whether it is wrapped.
void announce(const Greeting& g) {
    std::cout << g.text() << '\n';
}

int main() {
    PlainGreeting plain("Ada");
    announce(plain);            // Hello, Ada

    Excited excited(plain);     // wrap it -- same interface, extra behaviour
    announce(excited);          // Hello, Ada!
    return 0;
}
