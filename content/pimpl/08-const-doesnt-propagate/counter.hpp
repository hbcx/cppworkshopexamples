#ifndef PIMPL_CONST_COUNTER_HPP
#define PIMPL_CONST_COUNTER_HPP

#include <memory>

class Counter {
public:
    Counter();
    ~Counter();
    int reads() const;
    int peekBad() const;  // declared const, but MUTATES through impl_ (the hole)
    int lookGood() const; // const and honest: reads without mutating

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
