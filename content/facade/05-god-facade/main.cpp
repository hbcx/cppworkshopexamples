#include <iostream>
#include <string>

// Three unrelated subsystems.
class Billing {
public:
    void charge(int cents) { std::cout << "billing: charged " << cents << " cents\n"; }
};

class Mailer {
public:
    void send(const std::string& to) { std::cout << "mailer: mail to " << to << "\n"; }
};

class Reporting {
public:
    void daily() { std::cout << "reporting: daily figures\n"; }
};

// ANTI-PATTERN: a god facade that fronts EVERY subsystem. It is coupled to all of
// them, so a change in any subsystem can force an edit here; every client that
// needs one operation drags in a class depending on all three; and it only
// grows, because there is no natural boundary telling anyone where to stop.
class AppFacade {
public:
    void charge(int cents) { billing_.charge(cents); }
    void welcome(const std::string& to) { mailer_.send(to); }
    void dailyReport() { reporting_.daily(); }
private:
    Billing billing_;
    Mailer mailer_;
    Reporting reporting_;
};

// FIX: one focused facade per area of the system. A client depends only on the
// facade it uses, and a change in billing touches only BillingFacade.
class BillingFacade {
public:
    void charge(int cents) { billing_.charge(cents); }
private:
    Billing billing_;
};

class NotificationFacade {
public:
    void welcome(const std::string& to) { mailer_.send(to); }
private:
    Mailer mailer_;
};

int main() {
    std::cout << "== god facade ==\n";
    AppFacade app;
    app.charge(1800);
    app.welcome("alice");
    app.dailyReport();

    std::cout << "== focused facades ==\n";
    BillingFacade billing;
    NotificationFacade notify;
    billing.charge(1800);
    notify.welcome("alice");
    return 0;
}
