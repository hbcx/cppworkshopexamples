#include <iostream>

// A counter that steps through the days of a week (0..6, wrapping). It shows the
// two forms of ++ and, crucially, the difference in what they return.

class DayCounter {
public:
    explicit DayCounter(int day = 0) : day_(day % 7) {}

    int day() const { return day_; }

    // Prefix ++c: increment, then return a REFERENCE to this object.
    DayCounter& operator++() {
        day_ = (day_ + 1) % 7;
        return *this;
    }

    // Postfix c++: the dummy int marks it as postfix (never passed). Save the old
    // value, increment, and return the old value BY COPY.
    DayCounter operator++(int) {
        DayCounter old = *this;   // remember the state before the change
        ++(*this);                // reuse the prefix version
        return old;               // return the previous value, by value
    }

private:
    int day_;
};

int main() {
    DayCounter c{5};

    // Prefix returns the NEW value:
    std::cout << "++c -> " << (++c).day() << "\n";   // 6

    // Postfix returns the OLD value, but the object still advances:
    std::cout << "c++ -> " << (c++).day() << "\n";   // 6  (old), c is now 0
    std::cout << "c is  " << c.day() << "\n";        // 0  (wrapped past 6)
    return 0;
}
