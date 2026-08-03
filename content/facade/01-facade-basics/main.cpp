#include <iostream>
#include <string>

// Subsystem: three independent parts a client would otherwise drive by hand, in
// the right order, passing the output of one into the next.
class Grinder {
public:
    std::string grind() const {
        std::cout << "grinder: grinding beans\n";
        return "grounds";
    }
};

class Boiler {
public:
    std::string heat() const {
        std::cout << "boiler: heating water to 92C\n";
        return "hot water";
    }
};

class Pump {
public:
    std::string brew(const std::string& grounds, const std::string& water) const {
        std::cout << "pump: forcing " << water << " through " << grounds << '\n';
        return "espresso";
    }
};

// Facade: one simple entry point that knows which parts to call, in what order,
// and how their results connect. The client no longer needs to know the
// subsystem exists.
class CoffeeMachine {
public:
    std::string makeEspresso() {
        std::cout << "-- making espresso --\n";
        const std::string g = grinder_.grind();
        const std::string w = boiler_.heat();
        return pump_.brew(g, w);
    }
private:
    Grinder grinder_;
    Boiler boiler_;
    Pump pump_;
};

int main() {
    CoffeeMachine machine;
    const std::string drink = machine.makeEspresso();   // one call, not three
    std::cout << "served: " << drink << '\n';
    return 0;
}
