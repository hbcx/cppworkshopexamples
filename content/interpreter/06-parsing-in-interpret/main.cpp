// Anti-pattern: parsing fused into interpretation.
//
// The Gang of Four separate the grammar's REPRESENTATION and interpretation from
// how a sentence is PARSED. Fusing them -- a single pass that scans text and
// computes an answer, leaving no tree behind -- works for one evaluation but
// gives up everything the tree buys you: you cannot reuse the parse, run a second
// operation over it (print, validate, optimise), or inspect it. Here the bad
// version evaluates in the scan; the good version parses to a tree first, which
// it can then both interpret AND print.

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <utility>

// ---- BAD: parse and evaluate in one pass; no tree, nothing reusable ----
int evalBad(const std::string& expr) {
    std::istringstream in(expr);
    int acc = 0;
    in >> acc;
    std::string op;
    int n = 0;
    while (in >> op >> n) { // left-to-right, computing as we scan
        if (op == "+") acc += n;
        else if (op == "-") acc -= n;
        else if (op == "*") acc *= n;
    }
    return acc; // the answer, and nothing else -- the structure is gone
}

// ---- GOOD: parse to a tree; the tree can be interpreted AND printed ----
class Expr {
public:
    virtual ~Expr() = default;
    virtual int interpret() const = 0;
    virtual void print() const = 0;
};
class Number : public Expr {
    int v_;
public:
    explicit Number(int v) : v_(v) {}
    int interpret() const override { return v_; }
    void print() const override { std::cout << v_; }
};
class BinOp : public Expr {
    char op_;
    std::unique_ptr<Expr> l_, r_;
public:
    BinOp(char op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op_(op), l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override {
        int a = l_->interpret(), b = r_->interpret();
        return op_ == '+' ? a + b : op_ == '-' ? a - b : a * b;
    }
    void print() const override {
        std::cout << "("; l_->print(); std::cout << ' ' << op_ << ' '; r_->print(); std::cout << ")";
    }
};

// A separate parse phase: text -> tree (left-associative, matching evalBad).
static std::unique_ptr<Expr> parse(const std::string& expr) {
    std::istringstream in(expr);
    int n = 0;
    in >> n;
    std::unique_ptr<Expr> node(new Number(n));
    std::string op;
    while (in >> op >> n)
        node.reset(new BinOp(op[0], std::move(node), std::unique_ptr<Expr>(new Number(n))));
    return node;
}

int main() {
    std::string src = "3 + 4 * 2"; // left-to-right: (3+4)*2 = 14

    std::cout << "bad:  " << evalBad(src) << "\n"; // just the number

    std::unique_ptr<Expr> tree = parse(src);
    std::cout << "good: "; tree->print(); std::cout << " = " << tree->interpret() << "\n";
    // The tree can be interpreted, printed, re-run over new inputs, or visited --
    // none of which evalBad's fused pass can do.
    return 0;
}
