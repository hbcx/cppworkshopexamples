// Chain of Responsibility basics: pass a request until someone handles it.
//
// Each handler in the chain either processes the request or forwards it to the
// next one, so the sender (main) does not know or care which handler ends up
// doing the work -- it just hands the request to the head of the chain. Here an
// expense approval escalates by amount: a team lead handles small sums, a
// manager larger ones, a director larger still, and anything past the end is
// reported as unhandled by the base's terminal branch.

#include <iostream>

struct Request {
    const char* what;
    int amount;
};

class Approver {
    Approver* next_ = nullptr;
public:
    virtual ~Approver() = default;

    // Fluent link: returns the just-linked handler so calls can chain.
    Approver* setNext(Approver* next) { next_ = next; return next; }

    // The chain step: handle it, or pass it on, or report it fell off the end.
    void handle(const Request& r) {
        if (canApprove(r)) {
            approve(r);
        } else if (next_) {
            next_->handle(r);
        } else {
            std::cout << r.what << " ($" << r.amount << "): nobody could approve\n";
        }
    }

protected:
    virtual bool canApprove(const Request& r) const = 0;
    virtual void approve(const Request& r) const = 0;
};

class TeamLead : public Approver {
    bool canApprove(const Request& r) const override { return r.amount < 100; }
    void approve(const Request& r) const override {
        std::cout << r.what << " ($" << r.amount << "): approved by team lead\n";
    }
};
class Manager : public Approver {
    bool canApprove(const Request& r) const override { return r.amount < 1000; }
    void approve(const Request& r) const override {
        std::cout << r.what << " ($" << r.amount << "): approved by manager\n";
    }
};
class Director : public Approver {
    bool canApprove(const Request& r) const override { return r.amount < 10000; }
    void approve(const Request& r) const override {
        std::cout << r.what << " ($" << r.amount << "): approved by director\n";
    }
};

int main() {
    TeamLead lead;
    Manager mgr;
    Director dir;
    lead.setNext(&mgr)->setNext(&dir); // lead -> manager -> director

    // Every request enters at the head; the chain routes it.
    lead.handle({ "stapler", 20 });
    lead.handle({ "monitor", 300 });
    lead.handle({ "server", 5000 });
    lead.handle({ "datacenter", 500000 }); // past the end -> unhandled
    return 0;
}
