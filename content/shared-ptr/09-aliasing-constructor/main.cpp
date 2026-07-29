#include <iostream>
#include <memory>
#include <string>
#include <utility>

// A Car owns an Engine as a member -- one object, one allocation. We want to
// hand a component a shared_ptr to just the Engine, without a second control
// block and without letting the Engine outlive (or predecease) its Car.
struct Engine {
    std::string name;
    explicit Engine(std::string n) : name(std::move(n)) { std::cout << "Engine built\n"; }
    ~Engine() { std::cout << "Engine scrapped\n"; }
};

struct Car {
    Engine engine;
    int wheels;
    Car(std::string e, int w) : engine(std::move(e)), wheels(w) { std::cout << "Car built\n"; }
    ~Car() { std::cout << "Car scrapped\n"; }
};

int main() {
    std::shared_ptr<Car> car = std::make_shared<Car>("V8", 4);

    // Aliasing constructor: shares Car's control block (same reference count),
    // but get() returns &car->engine. No new allocation, no new control block.
    std::shared_ptr<Engine> engine(car, &car->engine);

    std::cout << "same owner, use_count = " << car.use_count() << '\n';   // 2
    std::cout << "engine handle sees: " << engine->name << '\n';

    // Hand the engine handle off and drop the car. The Car is NOT scrapped here:
    // the aliasing shared_ptr still owns the control block, so the whole Car --
    // and therefore the Engine subobject inside it -- stays alive.
    car.reset();
    std::cout << "after car.reset(), engine still valid: " << engine->name << '\n';
    std::cout << "engine use_count = " << engine.use_count() << '\n';      // 1

    std::cout << "leaving main\n";
    return 0;   // last owner drops -> the whole Car is destroyed now, in order
}
