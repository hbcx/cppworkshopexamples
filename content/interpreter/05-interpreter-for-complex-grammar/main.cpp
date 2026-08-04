// Anti-pattern: Interpreter for a grammar that outgrows it.
//
// Interpreter fits SMALL grammars. This example already shows the strain: even
// basic arithmetic with precedence and parentheses needs a node class per
// operator (Number, Add, Sub, Mul, Div) AND a hand-written recursive-descent
// parser of three mutually-recursive functions -- and that is the easy case.
// Each feature you add (unary minus, exponent with right-associativity,
// comparisons, function calls, variables, statements) multiplies both the class
// count and the parser, and a real language becomes unmaintainable by hand. Past
// a small grammar, reach for a parser generator or an existing expression
// library instead of growing this.

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
class Sub : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Sub(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() - r_->interpret(); }
};
class Mul : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Mul(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() * r_->interpret(); }
};
class Div : public Expr {
    std::unique_ptr<Expr> l_, r_;
public:
    Div(std::unique_ptr<Expr> l, std::unique_ptr<Expr> r) : l_(std::move(l)), r_(std::move(r)) {}
    int interpret() const override { return l_->interpret() / r_->interpret(); }
};

// A recursive-descent parser: three mutually-recursive functions just to get
// precedence and parentheses right for FOUR operators.
class Parser {
    std::vector<std::string> toks_;
    std::size_t pos_ = 0;
    const std::string& peek() const {
        static const std::string end;
        return pos_ < toks_.size() ? toks_[pos_] : end;
    }
    std::string take() { return toks_[pos_++]; }

    std::unique_ptr<Expr> parseExpr() { // lowest precedence: + and -
        std::unique_ptr<Expr> node = parseTerm();
        while (peek() == "+" || peek() == "-") {
            std::string op = take();
            std::unique_ptr<Expr> rhs = parseTerm();
            if (op == "+") node.reset(new Add(std::move(node), std::move(rhs)));
            else node.reset(new Sub(std::move(node), std::move(rhs)));
        }
        return node;
    }
    std::unique_ptr<Expr> parseTerm() { // higher precedence: * and /
        std::unique_ptr<Expr> node = parseFactor();
        while (peek() == "*" || peek() == "/") {
            std::string op = take();
            std::unique_ptr<Expr> rhs = parseFactor();
            if (op == "*") node.reset(new Mul(std::move(node), std::move(rhs)));
            else node.reset(new Div(std::move(node), std::move(rhs)));
        }
        return node;
    }
    std::unique_ptr<Expr> parseFactor() { // number or ( expr )
        if (peek() == "(") {
            take(); // '('
            std::unique_ptr<Expr> node = parseExpr();
            take(); // ')'
            return node;
        }
        return std::unique_ptr<Expr>(new Number(std::stoi(take())));
    }

public:
    explicit Parser(const std::string& src) {
        std::istringstream in(src);
        std::string t;
        while (in >> t) toks_.push_back(t);
    }
    std::unique_ptr<Expr> parse() { return parseExpr(); }
};

int main() {
    Parser p1("2 + 3 * 4"); // precedence: 2 + (3*4) = 14
    std::cout << "2 + 3 * 4 = " << p1.parse()->interpret() << "\n";

    Parser p2("( 2 + 3 ) * 4"); // parentheses: (2+3)*4 = 20
    std::cout << "( 2 + 3 ) * 4 = " << p2.parse()->interpret() << "\n";
    return 0;
}
