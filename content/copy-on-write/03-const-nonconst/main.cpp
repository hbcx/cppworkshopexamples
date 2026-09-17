#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

class CowText {
    struct Data {
        std::string value;
        int refs = 1;
    };
    Data* data_;
    void release() { if (--data_->refs == 0) delete data_; }
    void detach() { if (data_->refs > 1) { --data_->refs; data_ = new Data{data_->value, 1}; } }
public:
    explicit CowText(std::string s) : data_(new Data{std::move(s), 1}) {}
    CowText(const CowText& o) : data_(o.data_) { ++data_->refs; }
    CowText& operator=(const CowText& o) { ++o.data_->refs; release(); data_ = o.data_; return *this; }
    ~CowText() { release(); }

    const std::string& read() const { return data_->value; }
    const void*        buffer() const { return data_; }

    // Read: const, returns BY VALUE, never detaches -- shares stay intact.
    char operator[](std::size_t i) const { return data_->value[i]; }

    // Write: non-const, detaches first, then modifies the private buffer.
    void set(std::size_t i, char c) {
        detach();
        data_->value[i] = c;
    }
};

int main() {
    CowText a{"cat"};
    CowText b = a;   // shared

    // Reading through const operator[] does NOT detach: still shared.
    std::cout << "a[0]=" << a[0] << " b[0]=" << b[0]
              << " still shared? " << (a.buffer() == b.buffer()) << '\n';   // c c 1

    b.set(0, 'h');   // a non-const write detaches b
    std::cout << "after b.set: a=" << a.read() << " b=" << b.read()
              << " shared? " << (a.buffer() == b.buffer()) << '\n';         // cat hat 0
    return 0;
}
