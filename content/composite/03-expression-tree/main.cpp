#include <iostream>
#include <memory>
#include <utility>

// Component: something that evaluates to a number.
struct Expr {
    virtual int eval() const = 0;
    virtual ~Expr() = default;
};

// Leaf: a literal value.
class Num : public Expr {
public:
    explicit Num(int v) : v_(v) {}
    int eval() const override { return v_; }
private:
    int v_;
};

// Composite: a binary operation holds two sub-expressions -- each itself a leaf
// or another operation -- and combines their results. The tree IS the
// expression, and eval() recurses through it uniformly: a Num and an Add are both
// just an Expr, so a node never asks what kind of child it holds.
class Add : public Expr {
public:
    Add(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : l_(std::move(l)), r_(std::move(r)) {}
    int eval() const override { return l_->eval() + r_->eval(); }
private:
    std::unique_ptr<Expr> l_;
    std::unique_ptr<Expr> r_;
};

class Mul : public Expr {
public:
    Mul(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : l_(std::move(l)), r_(std::move(r)) {}
    int eval() const override { return l_->eval() * r_->eval(); }
private:
    std::unique_ptr<Expr> l_;
    std::unique_ptr<Expr> r_;
};

int main() {
    // Build the tree for (2 + 3) * 4 and evaluate it.
    std::unique_ptr<Expr> tree =
        std::make_unique<Mul>(
            std::make_unique<Add>(std::make_unique<Num>(2), std::make_unique<Num>(3)),
            std::make_unique<Num>(4));

    std::cout << "(2 + 3) * 4 = " << tree->eval() << "\n";   // 20
    return 0;
}
