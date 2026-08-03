#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Widget {
    virtual std::string draw() const = 0;
    virtual ~Widget() = default;
};

class Button : public Widget {
public:
    explicit Button(std::string label) : label_(std::move(label)) {}
    std::string draw() const override { return "[" + label_ + "]"; }
private:
    std::string label_;
};

// ANTI-PATTERN: a decorator that holds the wrapped widget by REFERENCE. It is
// safe only while something else keeps that widget alive. Build the chain from a
// local or a temporary -- the common case in a factory -- and the reference
// dangles the moment that local dies.
class BorderBad : public Widget {
public:
    explicit BorderBad(const Widget& inner) : inner_(inner) {}
    std::string draw() const override { return "|" + inner_.draw() + "|"; }
private:
    const Widget& inner_;
};

// std::unique_ptr<Widget> makeBad() {
//     Button b("ok");                              // local, dies when makeBad returns
//     return std::make_unique<BorderBad>(b);       // decorator holds a dangling ref -- UB
// }

// FIX: the decorator OWNS the wrapped widget via unique_ptr, so the chain keeps
// every layer alive for as long as the chain itself lives -- a factory can build
// and return it safely.
class BorderGood : public Widget {
public:
    explicit BorderGood(std::unique_ptr<Widget> inner) : inner_(std::move(inner)) {}
    std::string draw() const override { return "|" + inner_->draw() + "|"; }
private:
    std::unique_ptr<Widget> inner_;
};

std::unique_ptr<Widget> makeGood() {
    return std::make_unique<BorderGood>(std::make_unique<Button>("ok"));
}

int main() {
    // The reference version is safe ONLY because the widget outlives the
    // decorator in this exact scope.
    Button b("ok");
    BorderBad framed(b);
    std::cout << framed.draw() << '\n';    // |[ok]|  -- fine, b still alive

    // The owning version can be built in a factory and returned with no dangling.
    std::unique_ptr<Widget> w = makeGood();
    std::cout << w->draw() << '\n';        // |[ok]|
    return 0;
}
