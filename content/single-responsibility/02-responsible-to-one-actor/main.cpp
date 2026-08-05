// SRP, sharpened: a module should be responsible to ONE actor.
//
// The classic payroll example. One Employee class carries three methods that
// belong to three different parts of the business:
//   calculatePay  -> Accounting (the CFO)
//   reportHours   -> HR (the COO)
//   save          -> the DBA
// Serving three actors from one class means a change asked for by one can
// silently break another. The bad version shows exactly that; the good version
// splits the code each actor owns.

#include <iostream>
#include <string>

struct EmployeeData {
    std::string name;
    int hoursWorked;
    double hourlyRate;
};

namespace bad {

// One class, three masters. calculatePay and reportHours SHARE regularHours.
class Employee {
public:
    explicit Employee(EmployeeData d) : d_(d) {}

    double calculatePay() const {                    // Accounting's method
        int reg = regularHours();
        int overtime = d_.hoursWorked - reg;
        return reg * d_.hourlyRate + overtime * d_.hourlyRate * 1.5;
    }

    int reportHours() const {                        // HR's method
        return regularHours();                       // leans on the SAME helper
    }

private:
    // The CFO asks to change how overtime is counted -> a dev edits THIS.
    // HR's report, which shares it, silently changes with no one asking HR.
    int regularHours() const {
        return d_.hoursWorked < 40 ? d_.hoursWorked : 40;
    }
    EmployeeData d_;
};

} // namespace bad

namespace good {

// Accounting owns pay, and its own copy of the overtime rule.
class PayCalculator {
public:
    double calculatePay(const EmployeeData& e) const {
        int reg = regularHours(e.hoursWorked);
        int overtime = e.hoursWorked - reg;
        return reg * e.hourlyRate + overtime * e.hourlyRate * 1.5;
    }
private:
    static int regularHours(int worked) { return worked < 40 ? worked : 40; }
};

// HR owns the hour report, and its own copy of the rule. Looks identical to
// Accounting's today -- but it changes only when HR asks.
class HourReporter {
public:
    int reportableHours(const EmployeeData& e) const {
        return regularHours(e.hoursWorked);
    }
private:
    static int regularHours(int worked) { return worked < 40 ? worked : 40; }
};

// The DBA owns persistence.
class EmployeeRepository {
public:
    void save(const EmployeeData& e) const {
        std::cout << "[db] saved " << e.name << " (" << e.hoursWorked << "h)\n";
    }
};

} // namespace good

int main() {
    EmployeeData emp{"Dana", 45, 25.0};

    // Bad: one class answering to Accounting, HR and the DBA at once.
    bad::Employee legacy(emp);
    std::cout << "one class:  pay=" << legacy.calculatePay()
              << "  hrHours=" << legacy.reportHours() << "\n";

    // Good: three modules, one per actor, over shared data.
    good::PayCalculator pay;
    good::HourReporter hr;
    good::EmployeeRepository repo;
    std::cout << "split:      pay=" << pay.calculatePay(emp)
              << "  hrHours=" << hr.reportableHours(emp) << "\n";
    repo.save(emp);
}
