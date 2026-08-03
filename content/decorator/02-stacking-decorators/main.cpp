#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Component.
struct Message {
    virtual std::string text() const = 0;
    virtual ~Message() = default;
};

// Concrete component.
class Raw : public Message {
public:
    explicit Raw(std::string body) : body_(std::move(body)) {}
    std::string text() const override { return body_; }
private:
    std::string body_;
};

// Base decorator: OWNS the wrapped Message via unique_ptr, so a whole chain
// manages its own lifetime -- destroying the outermost decorator destroys every
// layer beneath it. Concrete decorators inherit this constructor and reach the
// wrapped text through inner().
class MessageDecorator : public Message {
public:
    explicit MessageDecorator(std::unique_ptr<Message> inner)
        : inner_(std::move(inner)) {}
protected:
    std::string inner() const { return inner_->text(); }
private:
    std::unique_ptr<Message> inner_;
};

// Prepends a "note: " tag.
class Quoted : public MessageDecorator {
public:
    using MessageDecorator::MessageDecorator;
    std::string text() const override { return "note: " + inner(); }
};

// Upper-cases everything it receives -- including whatever a lower layer added.
class Shouted : public MessageDecorator {
public:
    using MessageDecorator::MessageDecorator;
    std::string text() const override {
        std::string s = inner();
        for (char& c : s) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return s;
    }
};

int main() {
    // Quote first, then shout: SHOUT sees the "note: " tag and upcases it too.
    std::unique_ptr<Message> a =
        std::make_unique<Shouted>(
            std::make_unique<Quoted>(
                std::make_unique<Raw>("ok")));
    std::cout << a->text() << '\n';   // NOTE: OK

    // Shout first, then quote: the tag is added last and stays lower-case.
    std::unique_ptr<Message> b =
        std::make_unique<Quoted>(
            std::make_unique<Shouted>(
                std::make_unique<Raw>("ok")));
    std::cout << b->text() << '\n';   // note: OK
    return 0;
}
