#include <functional>
#include <iostream>
#include <utility>
#include <vector>

// The concrete dependents.
struct EmailService {
    void send(int orderId) { std::cout << "email for order " << orderId << "\n"; }
};
struct Analytics {
    void track(int orderId) { std::cout << "analytics for order " << orderId << "\n"; }
};

// ANTI-PATTERN: the subject hardcodes calls to each concrete dependent. Adding a
// reaction (SMS, inventory, audit) means editing this class, it cannot be reused
// or tested without those exact services, and it is coupled to all of them -- the
// very coupling Observer exists to remove.
class OrderServiceBad {
public:
    OrderServiceBad(EmailService& e, Analytics& a) : email_(e), analytics_(a) {}
    void placeOrder(int id) {
        std::cout << "order placed: " << id << "\n";
        email_.send(id);          // hardcoded dependent
        analytics_.track(id);     // hardcoded dependent -- a third means editing here
    }
private:
    EmailService& email_;
    Analytics& analytics_;
};

// FIX: the subject broadcasts to an observer list and knows none of its reactors.
// A new reaction is a new subscriber; the order service never changes.
class OrderServiceGood {
public:
    void onPlaced(std::function<void(int)> h) { handlers_.push_back(std::move(h)); }
    void placeOrder(int id) {
        std::cout << "order placed: " << id << "\n";
        for (auto& h : handlers_) h(id);
    }
private:
    std::vector<std::function<void(int)>> handlers_;
};

int main() {
    EmailService email;
    Analytics analytics;

    std::cout << "== hardcoded dependents ==\n";
    OrderServiceBad bad(email, analytics);
    bad.placeOrder(1);

    std::cout << "== observer list ==\n";
    OrderServiceGood good;
    good.onPlaced([&](int id) { email.send(id); });
    good.onPlaced([&](int id) { analytics.track(id); });
    // a third reaction is one more subscriber, with no edit to OrderServiceGood:
    good.onPlaced([](int id) { std::cout << "audit for order " << id << "\n"; });
    good.placeOrder(2);
    return 0;
}
