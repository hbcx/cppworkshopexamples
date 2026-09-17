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

    // Make sure this value owns a private buffer before it is modified.
    void detach() {
        if (data_->refs > 1) {
            --data_->refs;
            data_ = new Data{data_->value, 1};
        }
    }
public:
    explicit CowText(std::string s) : data_(new Data{std::move(s), 1}) {}
    CowText(const CowText& o) : data_(o.data_) { ++data_->refs; }
    CowText& operator=(const CowText& o) { ++o.data_->refs; release(); data_ = o.data_; return *this; }
    ~CowText() { release(); }

    const std::string& read() const { return data_->value; }
    const void*        buffer() const { return data_; }

    // A mutator: detach first, then modify the now-private buffer.
    void append(char c) {
        detach();
        data_->value.push_back(c);
    }
};

int main() {
    CowText a{"ab"};
    const void* before = a.buffer();
    a.append('c');   // a is unique -> modify in place, same buffer
    std::cout << "unique append kept the same buffer? " << (a.buffer() == before) << '\n';   // 1
    std::cout << "a = " << a.read() << '\n';                                                  // abc

    CowText b = a;                 // now the buffer is shared
    const void* shared = a.buffer();
    b.append('d');   // b is shared -> detach first, new buffer
    std::cout << "shared append detached b? " << (b.buffer() != shared) << '\n';              // 1
    std::cout << "a = " << a.read() << ", b = " << b.read() << '\n';                          // abc, abcd
    std::cout << "a still on the shared buffer? " << (a.buffer() == shared) << '\n';          // 1
    return 0;
}
