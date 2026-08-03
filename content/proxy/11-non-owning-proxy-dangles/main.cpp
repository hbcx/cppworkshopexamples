#include <iostream>
#include <memory>
#include <utility>

struct Sensor {
    virtual int read() const = 0;
    virtual ~Sensor() = default;
};

class RealSensor : public Sensor {
public:
    explicit RealSensor(int v) : v_(v) {}
    int read() const override { return v_; }
private:
    int v_;
};

// ANTI-PATTERN: a proxy that holds its subject by reference. It is valid only
// while some outside owner keeps that subject alive. Build the proxy in a factory
// over a local subject and the reference dangles the moment the factory returns.
class RefProxy : public Sensor {
public:
    explicit RefProxy(const Sensor& s) : s_(s) {}
    int read() const override { return s_.read(); }
private:
    const Sensor& s_;
};

// std::unique_ptr<Sensor> makeBad() {
//     RealSensor local(42);                        // dies when makeBad returns
//     return std::make_unique<RefProxy>(local);    // proxy left holding a dangling ref -- UB
// }

// FIX: the proxy OWNS its subject, so the proxy keeps it alive for as long as the
// proxy lives -- a factory can build and return it safely.
class OwningProxy : public Sensor {
public:
    explicit OwningProxy(std::unique_ptr<Sensor> s) : s_(std::move(s)) {}
    int read() const override { return s_->read(); }
private:
    std::unique_ptr<Sensor> s_;
};

std::unique_ptr<Sensor> makeGood() {
    return std::make_unique<OwningProxy>(std::make_unique<RealSensor>(42));
}

int main() {
    // Safe use of the reference proxy: the subject outlives it in this scope.
    RealSensor sensor(7);
    RefProxy ref(sensor);
    std::cout << "ref proxy:    " << ref.read() << "\n";     // 7

    // The owning proxy can be built in a factory and returned, with no dangling.
    std::unique_ptr<Sensor> p = makeGood();
    std::cout << "owning proxy: " << p->read() << "\n";      // 42
    return 0;
}
