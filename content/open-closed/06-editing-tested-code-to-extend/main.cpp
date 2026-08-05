// Anti-pattern: editing tested, working code to extend it.
//
// PricingService::price started with two tiers and grew by adding a branch for
// each new one. Every edit reopens a class that already works and risks its
// tested paths -- the "closed for modification" half of OCP, violated.
//
// The fix moves each tier behind a PricingRule the service looks up, so a new
// tier is a new rule object and the service is never edited again.

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

// ---------- one function edited for every new tier ----------
namespace bad {

class PricingService {
public:
    long price(long base, const std::string& tier) const {
        if (tier == "regular") return base;
        else if (tier == "premium") return base * 9 / 10; // added later
        else if (tier == "vip") return base * 8 / 10;     // added later, again
        return base;
    }
};

} // namespace bad

// ---------- each tier is its own rule ----------
namespace good {

class PricingRule {
public:
    virtual ~PricingRule() {}
    virtual long apply(long base) const = 0;
};

class Regular : public PricingRule {
public:
    long apply(long base) const override { return base; }
};
class Premium : public PricingRule {
public:
    long apply(long base) const override { return base * 9 / 10; }
};
class Vip : public PricingRule {
public:
    long apply(long base) const override { return base * 8 / 10; }
};

// Closed: it consults whatever rules it was given and is never edited to add one.
class PricingService {
public:
    void addTier(const std::string& name, std::unique_ptr<PricingRule> r) {
        rules_[name] = std::move(r);
    }
    long price(long base, const std::string& tier) const {
        std::unordered_map<std::string, std::unique_ptr<PricingRule> >::const_iterator it =
            rules_.find(tier);
        return it == rules_.end() ? base : it->second->apply(base);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<PricingRule> > rules_;
};

} // namespace good

int main() {
    bad::PricingService legacy;
    std::cout << "[bad]  vip 1000 -> " << legacy.price(1000, "vip") << "\n";

    good::PricingService svc;
    svc.addTier("regular", std::unique_ptr<good::PricingRule>(new good::Regular));
    svc.addTier("premium", std::unique_ptr<good::PricingRule>(new good::Premium));
    svc.addTier("vip", std::unique_ptr<good::PricingRule>(new good::Vip));
    std::cout << "[good] vip 1000 -> " << svc.price(1000, "vip") << "\n";

    // EXTENSION: a new tier is a new object, no edit to PricingService.
    class Student : public good::PricingRule {
    public:
        long apply(long base) const override { return base * 7 / 10; }
    };
    svc.addTier("student", std::unique_ptr<good::PricingRule>(new Student));
    std::cout << "[good] student 1000 -> " << svc.price(1000, "student") << "\n";
}
