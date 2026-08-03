#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Text {
    virtual std::string render() const = 0;
    virtual ~Text() = default;
};

class Plain : public Text {
public:
    explicit Plain(std::string s) : s_(std::move(s)) {}
    std::string render() const override { return s_; }
private:
    std::string s_;
};

// ANTI-PATTERN: one decorator that does THREE things at once -- tag, upper-case,
// and exclaim. It cannot be reused for just one of them, its internal order is
// frozen, and turning any part off means a new flag or a new class. A fat
// decorator is a fat class wearing the pattern's clothes.
class FancyBad : public Text {
public:
    explicit FancyBad(std::unique_ptr<Text> inner) : inner_(std::move(inner)) {}
    std::string render() const override {
        std::string s = inner_->render();
        for (char& c : s) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return "note: " + s + "!";
    }
private:
    std::unique_ptr<Text> inner_;
};

// FIX: one responsibility per decorator, composed to taste.
class TextDecorator : public Text {
public:
    explicit TextDecorator(std::unique_ptr<Text> inner) : inner_(std::move(inner)) {}
protected:
    std::string inner() const { return inner_->render(); }
private:
    std::unique_ptr<Text> inner_;
};

class Tag : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override { return "note: " + inner(); }
};

class Upper : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override {
        std::string s = inner();
        for (char& c : s) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return s;
    }
};

class Bang : public TextDecorator {
public:
    using TextDecorator::TextDecorator;
    std::string render() const override { return inner() + "!"; }
};

int main() {
    std::unique_ptr<Text> fat = std::make_unique<FancyBad>(std::make_unique<Plain>("ok"));
    std::cout << "fat:       " << fat->render() << '\n';        // note: OK!

    // Same result, but from single-responsibility decorators.
    std::unique_ptr<Text> composed =
        std::make_unique<Tag>(
            std::make_unique<Bang>(
                std::make_unique<Upper>(std::make_unique<Plain>("ok"))));
    std::cout << "composed:  " << composed->render() << '\n';   // note: OK!

    // ...and now each piece is reusable on its own -- impossible with the fat one.
    std::unique_ptr<Text> loud =
        std::make_unique<Bang>(std::make_unique<Upper>(std::make_unique<Plain>("hi")));
    std::cout << "just loud: " << loud->render() << '\n';       // HI!
    return 0;
}
