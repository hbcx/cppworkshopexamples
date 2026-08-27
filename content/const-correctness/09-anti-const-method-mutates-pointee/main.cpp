#include <iostream>

// Anti-pattern: a const member function that mutates through a pointer member.
// const applies to the members themselves, not to what a pointer member points
// at. So a const method may not repoint the pointer, but it CAN freely change
// the pointee -- the object looks const while its data changes underneath. That
// silently breaks "logical const" and the thread-safety callers assume of const.

class Widget {
public:
    explicit Widget(int* external) : data_(external) {}

    // Marked const, and it honestly changes no Widget member: data_ still points
    // where it did. But it writes through the pointer -- the const is skin deep.
    void touch() const {
        data_[0] = 42;          // allowed: const does not reach the pointee
        // data_ = nullptr;     // THIS would be an error: repointing a member
    }

    int peek() const { return data_[0]; }

private:
    int* data_;                 // raw pointer member
};

int main() {
    int storage[1] = {7};
    const Widget w(storage);    // a CONST widget

    std::cout << "before: storage[0] = " << storage[0] << "\n";   // 7
    w.touch();                  // a const method... yet it mutates the pointee
    std::cout << "after:  storage[0] = " << storage[0] << "\n";   // 42

    // Fixes, depending on intent:
    //  - if the data is logically part of the object, store it BY VALUE (or in a
    //    std::vector member) so const reaches it;
    //  - if a const method truly must not write, mark the pointee const --
    //    const int* data_; then data_[0] = 42 fails to compile;
    //  - std::experimental::propagate_const wraps a pointer so const propagates.
    return 0;
}
