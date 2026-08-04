// Interpreter basics: one class per grammar rule.
//
// A tiny language of boolean expressions. Each rule of the grammar is a class:
// Constant is a terminal (a leaf), And/Or/Not are non-terminals (they combine
// sub-expressions). A sentence in the language is a tree of these objects, and
// interpret() on the root recursively interprets the whole tree. The operation
// lives ON the grammar classes -- each node knows how to interpret itself --
// which is the shape of the pattern.

#include <iostream>
#include <memory>
#include <utility>

class BoolExpr {
public:
    virtual ~BoolExpr() = default;
    virtual bool interpret() const = 0;
};

// Terminal expression: a literal.
class Constant : public BoolExpr {
    bool value_;
public:
    explicit Constant(bool v) : value_(v) {}
    bool interpret() const override { return value_; }
};

// Non-terminal expressions: combine sub-expressions.
class And : public BoolExpr {
    std::unique_ptr<BoolExpr> lhs_, rhs_;
public:
    And(std::unique_ptr<BoolExpr> l, std::unique_ptr<BoolExpr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool interpret() const override { return lhs_->interpret() && rhs_->interpret(); }
};
class Or : public BoolExpr {
    std::unique_ptr<BoolExpr> lhs_, rhs_;
public:
    Or(std::unique_ptr<BoolExpr> l, std::unique_ptr<BoolExpr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool interpret() const override { return lhs_->interpret() || rhs_->interpret(); }
};
class Not : public BoolExpr {
    std::unique_ptr<BoolExpr> e_;
public:
    explicit Not(std::unique_ptr<BoolExpr> e) : e_(std::move(e)) {}
    bool interpret() const override { return !e_->interpret(); }
};

// Small helpers to build the tree readably.
using Expr = std::unique_ptr<BoolExpr>;
static Expr con(bool v) { return Expr(new Constant(v)); }
static Expr and_(Expr a, Expr b) { return Expr(new And(std::move(a), std::move(b))); }
static Expr or_(Expr a, Expr b) { return Expr(new Or(std::move(a), std::move(b))); }
static Expr not_(Expr a) { return Expr(new Not(std::move(a))); }

int main() {
    // (true AND false) OR (NOT false)
    Expr expr = or_(and_(con(true), con(false)), not_(con(false)));
    std::cout << "(true AND false) OR (NOT false) = "
              << (expr->interpret() ? "true" : "false") << "\n"; // true
    return 0;
}
