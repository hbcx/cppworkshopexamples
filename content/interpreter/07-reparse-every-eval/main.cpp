// Anti-pattern: re-parsing the expression on every evaluation.
//
// The whole point of turning text into a tree is that the tree is reusable: parse
// once, interpret many. Re-parsing the source string on each evaluation throws
// that away -- it pays the parse cost per record instead of once. Over a large
// dataset that is a real, avoidable slowdown. Here a parse counter makes it
// visible: the bad loop parses the rule 1000 times, the good loop parses it once
// and interprets the cached tree 1000 times.

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <utility>

int g_parses = 0; // counts how many times the rule text is parsed

class Record {
    std::map<std::string, int> f_;
public:
    void set(const std::string& k, int v) { f_[k] = v; }
    int get(const std::string& k) const {
        std::map<std::string, int>::const_iterator it = f_.find(k);
        return it != f_.end() ? it->second : 0;
    }
};

class Rule {
public:
    virtual ~Rule() = default;
    virtual bool eval(const Record& r) const = 0;
};
class Greater : public Rule {
    std::string field_;
    int n_;
public:
    Greater(std::string f, int n) : field_(std::move(f)), n_(n) {}
    bool eval(const Record& r) const override { return r.get(field_) > n_; }
};

// Parse "field > n" into a Rule tree. (Counts each parse.)
static std::unique_ptr<Rule> parse(const std::string& src) {
    ++g_parses;
    std::istringstream in(src);
    std::string field, op;
    int n = 0;
    in >> field >> op >> n;
    return std::unique_ptr<Rule>(new Greater(field, n));
}

int main() {
    const std::string ruleText = "age > 17";
    const int N = 1000;
    int eligible;

    // ---- BAD: parse the rule text on every record ----
    eligible = 0;
    for (int i = 0; i < N; ++i) {
        Record r; r.set("age", i % 40);
        std::unique_ptr<Rule> rule = parse(ruleText); // re-parsed every iteration
        if (rule->eval(r)) ++eligible;
    }
    std::cout << "bad:  eligible=" << eligible << ", parses=" << g_parses << "\n";

    // ---- GOOD: parse once, interpret the cached tree many times ----
    g_parses = 0;
    std::unique_ptr<Rule> rule = parse(ruleText); // parsed a single time
    eligible = 0;
    for (int i = 0; i < N; ++i) {
        Record r; r.set("age", i % 40);
        if (rule->eval(r)) ++eligible;
    }
    std::cout << "good: eligible=" << eligible << ", parses=" << g_parses << "\n";
    return 0;
}
