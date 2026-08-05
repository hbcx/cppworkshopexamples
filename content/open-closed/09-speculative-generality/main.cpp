// Anti-pattern: speculative generality (over-applying OCP).
//
// OCP is for change that actually comes. Building extension points, factories
// and interfaces for variation that never arrives is the opposite mistake:
// indirection and ceremony wrapped around ONE implementation.
//
// The bad side is a full strategy-plus-factory tower to say "Hello, <name>".
// The fix is the concrete version; add the seam when a SECOND case shows up.

#include <iostream>
#include <memory>
#include <string>
#include <utility>

// ---------- a framework for a single behaviour ----------
namespace bad {

class GreetingStrategy {
public:
    virtual ~GreetingStrategy() {}
    virtual std::string greet(const std::string& name) const = 0;
};

class EnglishGreeting : public GreetingStrategy {
public:
    std::string greet(const std::string& name) const override {
        return "Hello, " + name;
    }
};

class GreetingStrategyFactory {
public:
    // The only strategy that will ever exist -- the abstraction earns nothing.
    std::unique_ptr<GreetingStrategy> create() const {
        return std::unique_ptr<GreetingStrategy>(new EnglishGreeting);
    }
};

class Greeter {
public:
    explicit Greeter(std::unique_ptr<GreetingStrategy> s) : strategy_(std::move(s)) {}
    std::string greet(const std::string& name) const { return strategy_->greet(name); }
private:
    std::unique_ptr<GreetingStrategy> strategy_;
};

} // namespace bad

// ---------- just the code you need ----------
namespace good {

std::string greet(const std::string& name) { return "Hello, " + name; }

} // namespace good

int main() {
    // The tower: factory -> strategy -> greeter, to produce one string.
    bad::GreetingStrategyFactory factory;
    bad::Greeter greeter(factory.create());
    std::cout << "[bad]  " << greeter.greet("Ada") << "\n";

    // The concrete version. Add an interface later, when a real second case
    // (another language, say) actually arrives -- not before.
    std::cout << "[good] " << good::greet("Ada") << "\n";
}
