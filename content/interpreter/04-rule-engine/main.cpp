// A rule engine: a predicate DSL over records.
//
// This is Interpreter as it earns its keep -- a small language of business rules
// evaluated over data. The "sentences" are boolean predicates built from field
// comparisons (age > 17) combined with And/Or/Not; the Context is a Record whose
// fields the comparisons look up. A rule is built once and run over many records,
// so eligibility, targeting, and filtering logic can live as data-driven trees
// instead of hard-coded if-chains scattered through the code.

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <utility>

class Record { // the Context
    std::map<std::string, int> fields_;
public:
    void set(const std::string& key, int value) { fields_[key] = value; }
    int get(const std::string& key) const {
        std::map<std::string, int>::const_iterator it = fields_.find(key);
        return it != fields_.end() ? it->second : 0;
    }
};

class Rule {
public:
    virtual ~Rule() = default;
    virtual bool eval(const Record& r) const = 0;
};

// Terminals: field comparisons.
class Greater : public Rule {
    std::string field_;
    int n_;
public:
    Greater(std::string f, int n) : field_(std::move(f)), n_(n) {}
    bool eval(const Record& r) const override { return r.get(field_) > n_; }
};
class Equals : public Rule {
    std::string field_;
    int n_;
public:
    Equals(std::string f, int n) : field_(std::move(f)), n_(n) {}
    bool eval(const Record& r) const override { return r.get(field_) == n_; }
};

// Non-terminals: combine rules.
class And : public Rule {
    std::unique_ptr<Rule> lhs_, rhs_;
public:
    And(std::unique_ptr<Rule> l, std::unique_ptr<Rule> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool eval(const Record& r) const override { return lhs_->eval(r) && rhs_->eval(r); }
};
class Or : public Rule {
    std::unique_ptr<Rule> lhs_, rhs_;
public:
    Or(std::unique_ptr<Rule> l, std::unique_ptr<Rule> r) : lhs_(std::move(l)), rhs_(std::move(r)) {}
    bool eval(const Record& r) const override { return lhs_->eval(r) || rhs_->eval(r); }
};

using RulePtr = std::unique_ptr<Rule>;
static RulePtr gt(const std::string& f, int n) { return RulePtr(new Greater(f, n)); }
static RulePtr eq(const std::string& f, int n) { return RulePtr(new Equals(f, n)); }
static RulePtr all(RulePtr a, RulePtr b) { return RulePtr(new And(std::move(a), std::move(b))); }
static RulePtr any(RulePtr a, RulePtr b) { return RulePtr(new Or(std::move(a), std::move(b))); }

static Record record(int age, int vip, int priority) {
    Record r;
    r.set("age", age);
    r.set("vip", vip);
    r.set("priority", priority);
    return r;
}

int main() {
    // Rule: age > 17 AND (vip == 1 OR priority > 5). Built once.
    RulePtr rule = all(gt("age", 17), any(eq("vip", 1), gt("priority", 5)));

    Record recs[] = {
        record(25, 0, 8), // adult, not vip, high priority -> true
        record(16, 1, 9), // minor -> false
        record(30, 0, 2), // adult, not vip, low priority -> false
    };
    const char* names[] = { "alice", "bob", "carol" };

    for (int i = 0; i < 3; ++i)
        std::cout << names[i] << ": " << (rule->eval(recs[i]) ? "eligible" : "no") << "\n";
    return 0;
}
