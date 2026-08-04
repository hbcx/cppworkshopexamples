// Parsing text into the tree, then interpreting it.
//
// The grammar classes are the REPRESENTATION; getting a sentence into that
// representation is parsing, a separate step. Here a reverse-Polish (postfix)
// expression is parsed into an Expr tree with a stack: operands push a Number,
// operators pop two sub-trees and push a combined node. Once built, the tree
// interprets itself as before. Keeping parse and interpret as two phases (rather
// than evaluating during the scan) is what lets the tree be reused, printed, or
// optimised -- see the mixing and re-parsing anti-patterns.

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

class Expr {
public:
    virtual ~Expr() = default;
    virtual int interpret() const = 0;
};

class Number : public Expr {
    int value_;
public:
    explicit Number(int v) : value_(v) {}
    int interpret() const override { return value_; }
};

class BinOp : public Expr {
protected:
    std::unique_ptr<Expr> lhs_, rhs_;
public:
    // public so the inherited constructor is usable; BinOp is still abstract
    // (interpret is pure virtual), so it cannot be instantiated directly.
    BinOp(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
};
class Add : public BinOp {
public:
    using BinOp::BinOp; // inherit the constructor
    int interpret() const override { return lhs_->interpret() + rhs_->interpret(); }
};
class Sub : public BinOp {
public:
    using BinOp::BinOp;
    int interpret() const override { return lhs_->interpret() - rhs_->interpret(); }
};
class Mul : public BinOp {
public:
    using BinOp::BinOp;
    int interpret() const override { return lhs_->interpret() * rhs_->interpret(); }
};

// Parse a postfix expression into a tree (assumes well-formed input here).
static std::unique_ptr<Expr> parseRPN(const std::string& input) {
    std::vector<std::unique_ptr<Expr>> stack;
    std::istringstream in(input);
    std::string tok;
    while (in >> tok) {
        if (tok == "+" || tok == "-" || tok == "*") {
            std::unique_ptr<Expr> r = std::move(stack.back()); stack.pop_back();
            std::unique_ptr<Expr> l = std::move(stack.back()); stack.pop_back();
            if (tok == "+")      stack.push_back(std::unique_ptr<Expr>(new Add(std::move(l), std::move(r))));
            else if (tok == "-") stack.push_back(std::unique_ptr<Expr>(new Sub(std::move(l), std::move(r))));
            else                 stack.push_back(std::unique_ptr<Expr>(new Mul(std::move(l), std::move(r))));
        } else {
            stack.push_back(std::unique_ptr<Expr>(new Number(std::stoi(tok))));
        }
    }
    return std::move(stack.back());
}

int main() {
    std::string a = "3 4 + 2 *"; // (3 + 4) * 2 = 14
    std::cout << a << "  =>  " << parseRPN(a)->interpret() << "\n";

    std::string b = "10 2 - 3 *"; // (10 - 2) * 3 = 24
    std::cout << b << "  =>  " << parseRPN(b)->interpret() << "\n";
    return 0;
}
