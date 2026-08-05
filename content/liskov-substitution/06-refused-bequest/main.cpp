// Anti-pattern: a refused bequest.
//
// Bird promises fly(), but Penguin cannot fly, so its override throws. migrate()
// calls fly() through a Bird reference: fine for a sparrow, a run-time throw for
// a penguin. The base promised an operation a subtype cannot deliver.
//
// The fix segregates the interface: only a FlyingBird has fly(), so the compiler
// rejects a penguin at the call instead of failing at run time.

#include <iostream>
#include <stdexcept>
#include <string>

// ---------- Penguin refuses an operation Bird promises ----------
namespace bad {

class Bird {
public:
    virtual ~Bird() {}
    virtual std::string name() const = 0;
    virtual std::string fly() const { return "flap flap"; }
};

class Sparrow : public Bird {
public:
    std::string name() const override { return "sparrow"; }
};

class Penguin : public Bird {
public:
    std::string name() const override { return "penguin"; }
    std::string fly() const override { throw std::logic_error("penguins cannot fly"); }
};

// Written against Bird -- assumes every bird can fly.
void migrate(const Bird& b) {
    try {
        std::string flight = b.fly(); // may throw before anything is printed
        std::cout << "[bad] " << b.name() << ": " << flight << "\n";
    } catch (const std::exception& e) {
        std::cout << "[bad] " << b.name() << ": ERROR " << e.what() << "\n";
    }
}

} // namespace bad

// ---------- flying lives in its own interface ----------
namespace good {

class Bird {
public:
    virtual ~Bird() {}
    virtual std::string name() const = 0;
};

class FlyingBird : public Bird {
public:
    virtual std::string fly() const = 0;
};

class Sparrow : public FlyingBird {
public:
    std::string name() const override { return "sparrow"; }
    std::string fly() const override { return "flap flap"; }
};

class Penguin : public Bird {
public:
    std::string name() const override { return "penguin"; }
    std::string swim() const { return "paddle paddle"; }
};

// Takes only birds that can actually fly -- a penguin cannot be passed here.
void migrate(const FlyingBird& b) {
    std::cout << "[good] " << b.name() << ": " << b.fly() << "\n";
}

} // namespace good

int main() {
    bad::Sparrow bs;
    bad::Penguin bp;
    bad::migrate(bs);
    bad::migrate(bp); // throws inside, caught and reported

    good::Sparrow gs;
    good::Penguin gp;
    good::migrate(gs);                 // only flyers reach migrate()
    std::cout << "[good] " << gp.name() << ": " << gp.swim() << "\n";
}
