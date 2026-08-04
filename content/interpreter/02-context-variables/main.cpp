// The Context: terminals that stand for variables.
//
// A language is only useful once its sentences can refer to inputs. The Context
// holds the bindings -- here, named boolean variables -- and interpret() takes it
// so that a Variable terminal looks its value up at evaluation time. The SAME
// expression tree can then be interpreted against different contexts to get
// different answers, which is the point: build the rule once, evaluate it against
// many inputs.

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <utility>

class Context {
    std::map<std::string, bool> vars_;
public:
    void set(const std::string& name, bool value) { vars_[name] = value; }
    bool get(const std::string& name) const {
        std::map<std::string, bool>::const_iterator it = vars_.find(name);
        return it != vars_.end() ? it->second : false;
    }
};

class BoolExpr {
public:
    virtual ~BoolExpr() = default;
    virtual bool interpret(const Context& ctx) const = 0;
};

// Terminal: looks itself up in the context at interpret time.
class Variable : public BoolExpr {
    std::string name_;
public:
    explicit Variable(std::string name) : name_(std::move(name)) {}
    bool interpret(const Context& ctx) const override { return ctx.get(name_); }
};

class And : public BoolExpr {
    std::unique_ptr<BoolExpr> lhs_, rhs_;
public:
    And(std::unique_ptr<BoolExpr> l, std::unique_ptr<BoolExpr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool interpret(const Context& ctx) const override { return lhs_->interpret(ctx) && rhs_->interpret(ctx); }
};
class Or : public BoolExpr {
    std::unique_ptr<BoolExpr> lhs_, rhs_;
public:
    Or(std::unique_ptr<BoolExpr> l, std::unique_ptr<BoolExpr> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool interpret(const Context& ctx) const override { return lhs_->interpret(ctx) || rhs_->interpret(ctx); }
};
class Not : public BoolExpr {
    std::unique_ptr<BoolExpr> e_;
public:
    explicit Not(std::unique_ptr<BoolExpr> e) : e_(std::move(e)) {}
    bool interpret(const Context& ctx) const override { return !e_->interpret(ctx); }
};

using Expr = std::unique_ptr<BoolExpr>;
static Expr var(const std::string& n) { return Expr(new Variable(n)); }
static Expr and_(Expr a, Expr b) { return Expr(new And(std::move(a), std::move(b))); }
static Expr or_(Expr a, Expr b) { return Expr(new Or(std::move(a), std::move(b))); }
static Expr not_(Expr a) { return Expr(new Not(std::move(a))); }

int main() {
    // (x OR y) AND NOT z -- built once, evaluated against two contexts.
    Expr rule = and_(or_(var("x"), var("y")), not_(var("z")));

    Context c1;
    c1.set("x", true); c1.set("y", false); c1.set("z", false);
    std::cout << "c1 {x=1,y=0,z=0}: " << (rule->interpret(c1) ? "true" : "false") << "\n"; // true

    Context c2;
    c2.set("x", false); c2.set("y", false); c2.set("z", false);
    std::cout << "c2 {x=0,y=0,z=0}: " << (rule->interpret(c2) ? "true" : "false") << "\n"; // false
    return 0;
}
