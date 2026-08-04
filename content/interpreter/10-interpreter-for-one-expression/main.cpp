// Anti-pattern: Interpreter for a single fixed expression.
//
// Interpreter earns its machinery when a language is evaluated at runtime with
// inputs that vary. Building the grammar-class apparatus to compute one fixed,
// hard-coded expression is pure ceremony -- there is no language, no input, no
// runtime variation, so the tree computes a constant that the expression itself
// would state directly. If the formula is fixed and known at compile time, just
// write it.

#include <iostream>
#include <memory>
#include <utility>

// ---- BAD: a grammar tree built to compute one constant expression ----
class Expr {
public:
    virtual ~Expr() = default;
    virtual int interpret() const = 0;
};
class Number : public Expr {
    int v_;
public:
    explicit Number(int v) : v_(v) {}
    int interpret() const override { return v_; }
};
class Add : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Add(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() + r_->interpret(); }
};

int main() {
    // BAD: three objects and a virtual dispatch to compute 2 + 3.
    std::unique_ptr<Expr> tree(new Add(
        std::unique_ptr<Expr>(new Number(2)),
        std::unique_ptr<Expr>(new Number(3))));
    std::cout << "bad:  " << tree->interpret() << "\n"; // 5

    // GOOD: it is a fixed expression -- just write it.
    int result = 2 + 3;
    std::cout << "good: " << result << "\n"; // 5
    return 0;
}
