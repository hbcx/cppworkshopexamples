#include <iostream>
#include <vector>

// Observer interface: every subscriber implements update(), which the subject
// calls when its state changes.
struct Observer {
    virtual void update(int temperature) = 0;
    virtual ~Observer() = default;
};

// Subject: holds a list of observers and notifies them all on a state change. It
// knows only the Observer interface, not the concrete reactors -- that decoupling
// is the point of the pattern.
class Thermometer {
public:
    void subscribe(Observer* o) { observers_.push_back(o); }
    void setTemperature(int t) {
        temperature_ = t;
        for (Observer* o : observers_) o->update(temperature_);   // one-to-many broadcast
    }
private:
    int temperature_ = 0;
    std::vector<Observer*> observers_;
};

// Concrete observers, each reacting in its own way to the same notification.
class Display : public Observer {
public:
    void update(int t) override { std::cout << "display: " << t << " C\n"; }
};

class Alarm : public Observer {
public:
    void update(int t) override {
        if (t > 30) std::cout << "alarm: too hot (" << t << " C)!\n";
    }
};

int main() {
    Thermometer thermo;
    Display display;
    Alarm alarm;
    thermo.subscribe(&display);
    thermo.subscribe(&alarm);

    thermo.setTemperature(25);   // display reacts; alarm stays silent
    thermo.setTemperature(35);   // both react
    return 0;
}
