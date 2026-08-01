#include <iostream>
#include <string>

// Six fine-grained collaborators.
struct Catalog   { int priceOf(const std::string&)  { return 100; } };
struct TaxRules  { int taxOn(int amount)            { return amount / 10; } };
struct Discounts { int off(int amount)              { return amount / 20; } };
struct Inventory { bool reserve(const std::string&) { return true; } };
struct Shipping  { int cost()                       { return 15; } };
struct Emailer   { void confirm(const std::string& who) { std::cout << "  email -> " << who << '\n'; } };

// Anti-pattern: the constructor takes six dependencies. That long list is the
// smell -- the class does pricing AND fulfilment AND notification, i.e. too many
// jobs. Each dependency is one more thing to wire, to mock in a test, and to
// reason about together.
class OrderServiceFat {
public:
    OrderServiceFat(Catalog& c, TaxRules& t, Discounts& d,
                    Inventory& i, Shipping& s, Emailer& e)
        : c_(c), t_(t), d_(d), i_(i), s_(s), e_(e) {}
    int total(const std::string& sku, const std::string& who) {
        int base = c_.priceOf(sku);
        int amount = base + t_.taxOn(base) - d_.off(base);
        i_.reserve(sku);
        amount += s_.cost();
        e_.confirm(who);
        return amount;
    }
private:
    Catalog& c_; TaxRules& t_; Discounts& d_;
    Inventory& i_; Shipping& s_; Emailer& e_;
};

// Fix: group the fine-grained collaborators into cohesive services. Pricing owns
// catalog + tax + discount; Fulfilment owns inventory + shipping + email. The
// order service depends on TWO things it can name, not six, and each facade is
// separately testable.
class Pricing {
public:
    Pricing(Catalog& c, TaxRules& t, Discounts& d) : c_(c), t_(t), d_(d) {}
    int quote(const std::string& sku) {
        int base = c_.priceOf(sku);
        return base + t_.taxOn(base) - d_.off(base);
    }
private:
    Catalog& c_; TaxRules& t_; Discounts& d_;
};

class Fulfilment {
public:
    Fulfilment(Inventory& i, Shipping& s, Emailer& e) : i_(i), s_(s), e_(e) {}
    int ship(const std::string& sku, const std::string& who) {
        i_.reserve(sku);
        e_.confirm(who);
        return s_.cost();
    }
private:
    Inventory& i_; Shipping& s_; Emailer& e_;
};

class OrderServiceLean {
public:
    OrderServiceLean(Pricing& p, Fulfilment& f) : p_(p), f_(f) {}
    int total(const std::string& sku, const std::string& who) {
        return p_.quote(sku) + f_.ship(sku, who);
    }
private:
    Pricing& p_; Fulfilment& f_;
};

int main() {
    Catalog c; TaxRules t; Discounts d; Inventory i; Shipping s; Emailer e;

    // Fat: six dependencies wired straight into one class.
    OrderServiceFat fat(c, t, d, i, s, e);
    int fatTotal = fat.total("book", "alice");
    std::cout << "fat  (6 deps) total = " << fatTotal << '\n';

    // Lean: two cohesive facades, same result.
    Pricing pricing(c, t, d);
    Fulfilment fulfilment(i, s, e);
    OrderServiceLean lean(pricing, fulfilment);
    int leanTotal = lean.total("book", "alice");
    std::cout << "lean (2 deps) total = " << leanTotal << '\n';
    return 0;
}
