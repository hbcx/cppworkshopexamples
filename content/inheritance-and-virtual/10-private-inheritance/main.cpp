#include <iostream>

// A Car is implemented in terms of an Engine -- it uses one, but is not one. Private
// inheritance expresses that: Engine's interface becomes private in Car.

class Engine {
public:
    void start() { running_ = true; }
    int rpm() const { return running_ ? 900 : 0; }
private:
    bool running_ = false;
};

class Car : private Engine {   // implemented-in-terms-of, NOT is-a
public:
    using Engine::rpm;         // selectively re-expose one base member

    void drive() {
        start();               // use the base internally (start() is private to outsiders)
        std::cout << "driving at " << rpm() << " rpm\n";
    }
};

int main() {
    Car car;
    car.drive();                       // driving at 900 rpm
    std::cout << "rpm(): " << car.rpm() << "\n";   // 900   (re-exposed via using)

    // car.start();     // would NOT compile: start() is private through private inheritance
    // Engine* e = &car; // would NOT compile: no accessible Car-to-Engine conversion
    return 0;
}
