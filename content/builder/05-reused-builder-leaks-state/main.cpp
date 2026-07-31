#include <iostream>
#include <string>
#include <utility>

struct Message {
    std::string to;
    std::string body;
    void print() const { std::cout << "to=" << to << " body=\"" << body << "\"\n"; }
};

// A fluent builder that accumulates state and never clears it on build().
class MessageBuilder {
public:
    MessageBuilder& to(std::string t) { to_ = std::move(t); return *this; }
    MessageBuilder& body(std::string b) { body_ = std::move(b); return *this; }
    Message build() const { return Message{to_, body_}; }
private:
    std::string to_;
    std::string body_;
};

int main() {
    // Anti-pattern: one builder reused for several messages.
    MessageBuilder shared;
    Message first = shared.to("ada").body("hello").build();
    Message second = shared.to("bob").build();   // body NOT set again...

    first.print();
    second.print();   // ...but "hello" leaks in from the first product

    std::cout << "-- fresh builder per message --\n";

    // Fix: a single-use builder per product; each starts from clean defaults.
    Message a = MessageBuilder().to("ada").body("hello").build();
    Message b = MessageBuilder().to("bob").build();   // body is empty, as intended

    a.print();
    b.print();
    return 0;
}
