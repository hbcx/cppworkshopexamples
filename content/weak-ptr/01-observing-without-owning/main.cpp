#include <iostream>
#include <memory>

class Sensor {
public:
    explicit Sensor(int id) : id_(id) { std::cout << "Sensor " << id_ << " online\n"; }
    ~Sensor() { std::cout << "Sensor " << id_ << " offline\n"; }
    int id() const { return id_; }
private:
    int id_;
};

int main() {
    std::weak_ptr<Sensor> observer;              // owns nothing yet

    auto owner = std::make_shared<Sensor>(1);
    observer = owner;                            // observe: does NOT raise use_count
    std::cout << "use_count with an observer = " << owner.use_count() << '\n';  // 1

    // lock() promotes to a shared_ptr while the object is alive.
    if (std::shared_ptr<Sensor> s = observer.lock()) {
        std::cout << "locked Sensor " << s->id() << ", use_count now " << s.use_count() << '\n';  // 2
    }
    std::cout << "after lock released, use_count = " << owner.use_count() << '\n';  // 1

    owner.reset();                               // the only owner goes -> object destroyed
    std::cout << "expired? " << (observer.expired() ? "yes" : "no") << '\n';       // yes
    std::cout << "lock() now returns " << (observer.lock() ? "a pointer" : "null") << '\n';  // null
    return 0;
}
