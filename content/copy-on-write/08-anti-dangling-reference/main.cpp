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
    void write(std::string s) { detach(); data_->value = std::move(s); }
};

int main() {
    CowText s{"hello"};
    const char& ref     = s.read()[0];   // reference into s's current buffer
    const void* refAddr = &s.read()[0];

    CowText copy = s;   // now shared: s and copy point at ONE buffer
    s.write("world");   // s writes -> detaches onto a NEW buffer

    // ref/refAddr still point into the OLD buffer (which `copy` holds), not s's
    // new one. Comparison only -- we never dereference a dangling pointer.
    std::cout << "ref still points into s's live buffer? "
              << (refAddr == &s.read()[0]) << '\n';                       // 0
    std::cout << "ref sees old data '" << ref
              << "', s now starts with '" << s.read()[0] << "'\n";        // h, w

    // The fix: do not keep a reference into a copy-on-write value across a copy
    // followed by a write. This reference invalidation is why std::string was
    // required to stop being copy-on-write in C++11.
    return 0;
}
