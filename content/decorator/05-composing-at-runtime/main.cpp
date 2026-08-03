#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Component: a text filter.
struct Filter {
    virtual std::string apply(const std::string& in) const = 0;
    virtual ~Filter() = default;
};

// The base of every chain: does nothing, just returns the input. Starting from
// an identity lets the builder below wrap zero or more decorators uniformly.
class Identity : public Filter {
public:
    std::string apply(const std::string& in) const override { return in; }
};

// Base decorator: owns the layer below and exposes it through pass().
class FilterDecorator : public Filter {
public:
    explicit FilterDecorator(std::unique_ptr<Filter> inner)
        : inner_(std::move(inner)) {}
protected:
    std::string pass(const std::string& in) const { return inner_->apply(in); }
private:
    std::unique_ptr<Filter> inner_;
};

class Trim : public FilterDecorator {
public:
    using FilterDecorator::FilterDecorator;
    std::string apply(const std::string& in) const override {
        std::string s = pass(in);
        const std::size_t b = s.find_first_not_of(' ');
        if (b == std::string::npos) return "";
        const std::size_t e = s.find_last_not_of(' ');
        return s.substr(b, e - b + 1);
    }
};

class Upper : public FilterDecorator {
public:
    using FilterDecorator::FilterDecorator;
    std::string apply(const std::string& in) const override {
        std::string s = pass(in);
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
        return s;
    }
};

class Exclaim : public FilterDecorator {
public:
    using FilterDecorator::FilterDecorator;
    std::string apply(const std::string& in) const override { return pass(in) + "!"; }
};

// Assemble a decorator chain from data known only at runtime -- a config list, a
// set of feature flags, a request's options. A compile-time subclass hierarchy
// cannot do this: the exact stack is not known until the loop runs.
std::unique_ptr<Filter> build(const std::vector<std::string>& features) {
    std::unique_ptr<Filter> f = std::make_unique<Identity>();
    for (const std::string& name : features) {
        if (name == "trim")         f = std::make_unique<Trim>(std::move(f));
        else if (name == "upper")   f = std::make_unique<Upper>(std::move(f));
        else if (name == "exclaim") f = std::make_unique<Exclaim>(std::move(f));
    }
    return f;
}

int main() {
    auto a = build({"trim", "upper"});
    std::cout << "[" << a->apply("  hello  ") << "]\n";   // [HELLO]

    auto b = build({"upper", "exclaim"});
    std::cout << "[" << b->apply("hello") << "]\n";       // [HELLO!]

    auto none = build({});
    std::cout << "[" << none->apply("  hi ") << "]\n";     // [  hi ]  (identity)
    return 0;
}
