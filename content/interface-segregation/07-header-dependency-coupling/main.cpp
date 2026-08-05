// Anti-pattern: a fat interface couples its clients at compile time.
//
// logCount() only needs an event count, but it depends on IAnalytics, whose
// other methods deal in Report and Chart. So the client is transitively coupled
// to those types and must rebuild when they change -- even though it never
// builds a report.
//
// The fix: depend on a narrow EventCounter that mentions no report or chart
// type, so the coupling is exactly what is used.

#include <iostream>
#include <string>

// "Heavy", unrelated types the fat interface drags in.
struct Report { std::string title; int rows; };
struct Chart { int bars; };

// ---------- the fat interface couples everyone to Report and Chart ----------
namespace bad {

class IAnalytics {
public:
    virtual ~IAnalytics() {}
    virtual int eventCount() const = 0;
    virtual Report buildReport() const = 0; // signatures reference Report...
    virtual Chart buildChart() const = 0;   // ...and Chart
};

class Analytics : public IAnalytics {
public:
    int eventCount() const override { return 42; }
    Report buildReport() const override { return Report{"weekly", 10}; }
    Chart buildChart() const override { return Chart{7}; }
};

// Needs only the count, yet depends on IAnalytics -> on Report and Chart too.
int logCount(const IAnalytics& a) { return a.eventCount(); }

} // namespace bad

// ---------- a narrow interface couples the client to nothing extra ----------
namespace good {

class EventCounter {
public:
    virtual ~EventCounter() {}
    virtual int eventCount() const = 0; // mentions no Report, no Chart
};

class Analytics : public EventCounter {
public:
    int eventCount() const override { return 42; }
    // buildReport / buildChart live behind their own narrow interfaces.
};

// Depends only on EventCounter -- a change to Report cannot touch it.
int logCount(const EventCounter& c) { return c.eventCount(); }

} // namespace good

int main() {
    bad::Analytics ba;
    std::cout << "[bad]  count = " << bad::logCount(ba) << "\n";

    good::Analytics ga;
    std::cout << "[good] count = " << good::logCount(ga) << "\n";
}
