#include <iostream>
#include <memory>
#include <vector>

// A tiny hierarchy of notifications. Different kinds live together as base
// pointers, and one loop sends them all.

class Notification {
public:
    virtual void send() const = 0;
    virtual ~Notification() = default;
};

class Email : public Notification {
public:
    explicit Email(const char* to) : to_(to) {}
    void send() const override { std::cout << "email -> " << to_ << "\n"; }
private:
    const char* to_;
};

class Sms : public Notification {
public:
    explicit Sms(const char* number) : number_(number) {}
    void send() const override { std::cout << "sms   -> " << number_ << "\n"; }
private:
    const char* number_;
};

int main() {
    std::vector<std::unique_ptr<Notification>> outbox;
    outbox.push_back(std::unique_ptr<Notification>(new Email("ada@example.com")));
    outbox.push_back(std::unique_ptr<Notification>(new Sms("+48 555 111")));
    outbox.push_back(std::unique_ptr<Notification>(new Email("bob@example.com")));

    // One loop, every kind -- each call dispatches to the real type:
    for (const auto& n : outbox)
        n->send();
    // email -> ada@example.com
    // sms   -> +48 555 111
    // email -> bob@example.com
    return 0;
}
