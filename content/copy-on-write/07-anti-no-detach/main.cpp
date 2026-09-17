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
    CowText& operator=(const CowText&) = delete;
    ~CowText() { release(); }
    const std::string& read() const { return data_->value; }

    // BUGGY: hands out a writable reference into the buffer WITHOUT detaching, so
    // a write through it lands in the SHARED bytes and bleeds into every copy.
    char& at_buggy(std::size_t i) { return data_->value[i]; }

    // FIXED: detach first, so the reference is into THIS value's private buffer.
    char& at_fixed(std::size_t i) { detach(); return data_->value[i]; }
};

int main() {
    {
        CowText a{"cat"};
        CowText b = a;             // shared buffer
        a.at_buggy(0) = 'h';       // writes into the SHARED buffer
        std::cout << "buggy: a=" << a.read() << " b=" << b.read() << '\n';   // hat hat  <- b changed too
    }
    {
        CowText a{"cat"};
        CowText b = a;
        a.at_fixed(0) = 'h';       // detaches a first
        std::cout << "fixed: a=" << a.read() << " b=" << b.read() << '\n';   // hat cat
    }
    return 0;
}
