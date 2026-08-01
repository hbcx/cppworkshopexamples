#include <iostream>
#include <string>

// A dependency that, in production, does real I/O (sends an email). Its
// interface is all the consumer knows about.
struct EmailGateway {
    virtual void send(const std::string& to, const std::string& body) = 0;
    virtual ~EmailGateway() = default;
};

// The unit under test: it contains logic worth testing (building and addressing
// the reset message) and a side effect (sending it). Injection lets us keep the
// logic and swap out the side effect.
class PasswordReset {
public:
    explicit PasswordReset(EmailGateway& gateway) : gateway_(gateway) {}
    void request(const std::string& user) {
        gateway_.send(user + "@corp.example",
                      "Reset your password, " + user);
    }
private:
    EmailGateway& gateway_;
};

// A test double: instead of sending, it records what it was asked to send, so a
// test can assert on it -- no network, no flakiness, fully deterministic.
struct FakeGateway : EmailGateway {
    std::string lastTo;
    std::string lastBody;
    int calls = 0;
    void send(const std::string& to, const std::string& body) override {
        lastTo = to;
        lastBody = body;
        ++calls;
    }
};

int main() {
    // Playing the role of a unit test: inject the fake, run the logic, assert on
    // what was captured.
    FakeGateway fake;
    PasswordReset service(fake);
    service.request("alice");

    std::cout << "calls: " << fake.calls << '\n';
    std::cout << "to:    " << fake.lastTo << '\n';
    std::cout << "body:  " << fake.lastBody << '\n';

    bool ok = fake.calls == 1
              && fake.lastTo == "alice@corp.example"
              && fake.lastBody == "Reset your password, alice";
    std::cout << "test " << (ok ? "PASSED" : "FAILED") << '\n';
    return 0;
}
