// A real object structure: an expression tree with two visitors.
//
// The canonical Visitor use is a tree of node types visited by different
// operations. Here an arithmetic expression -- Number, Add, Mul -- is walked by
// a Printer (renders it) and an Evaluator (computes its value). Both are visitors
// over the same nodes, added without changing the nodes. The tree is recursive,
// so the visitors recurse by calling accept on the children. Because visit
// returns void, the Evaluator carries its result in a member -- the classic way
// a visitor accumulates a value over a structure.

#include <iostream>
#include <memory>
#include <utility>

class Number;
class Add;
class Mul;

class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual void visit(const Number& n) = 0;
    virtual void visit(const Add& a) = 0;
    virtual void visit(const Mul& m) = 0;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(ExprVisitor& v) const = 0;
};

class Number : public Expr {
    int value_;
public:
    explicit Number(int v) : value_(v) {}
    int value() const { return value_; }
    void accept(ExprVisitor& v) const override { v.visit(*this); }
};
class Add : public Expr {
    std::unique_ptr<Expr> lhs_, rhs_;
public:
    Add(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    const Expr& lhs() const { return *lhs_; }
    const Expr& rhs() const { return *rhs_; }
    void accept(ExprVisitor& v) const override { v.visit(*this); }
};
class Mul : public Expr {
    std::unique_ptr<Expr> lhs_, rhs_;
public:
    Mul(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    const Expr& lhs() const { return *lhs_; }
    const Expr& rhs() const { return *rhs_; }
    void accept(ExprVisitor& v) const override { v.visit(*this); }
};

// Operation 1: evaluate. Result held in a member; children evaluated recursively.
class Evaluator : public ExprVisitor {
    int result_ = 0;
public:
    int result() const { return result_; }
    void visit(const Number& n) override { result_ = n.value(); }
    void visit(const Add& a) override {
        Evaluator l, r;
        a.lhs().accept(l);
        a.rhs().accept(r);
        result_ = l.result() + r.result();
    }
    void visit(const Mul& m) override {
        Evaluator l, r;
        m.lhs().accept(l);
        m.rhs().accept(r);
        result_ = l.result() * r.result();
    }
};

// Operation 2: pretty-print. Same nodes, no change to them.
class Printer : public ExprVisitor {
public:
    void visit(const Number& n) override { std::cout << n.value(); }
    void visit(const Add& a) override {
        std::cout << "("; a.lhs().accept(*this); std::cout << " + "; a.rhs().accept(*this); std::cout << ")";
    }
    void visit(const Mul& m) override {
        std::cout << "("; m.lhs().accept(*this); std::cout << " * "; m.rhs().accept(*this); std::cout << ")";
    }
};

int main() {
    // (2 + 3) * 4
    std::unique_ptr<Expr> expr(new Mul(
        std::unique_ptr<Expr>(new Add(
            std::unique_ptr<Expr>(new Number(2)),
            std::unique_ptr<Expr>(new Number(3)))),
        std::unique_ptr<Expr>(new Number(4))));

    Printer printer;
    expr->accept(printer);
    std::cout << " = ";

    Evaluator eval;
    expr->accept(eval);
    std::cout << eval.result() << "\n";
    return 0;
}
