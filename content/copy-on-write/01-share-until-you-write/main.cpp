#include <iostream>
#include <string>
#include <utility>

// A copy-on-write text value. Several copies share ONE buffer (a refcounted
// control block); a copy just bumps the reference count. Only a write makes a
// private copy -- see write() -- so copying is O(1) until someone modifies.
class CowText {
    struct Data {
        std::string value;
        int refs = 1;
    };
    Data* data_;
    void release() { if (--data_->refs == 0) delete data_; }
public:
    explicit CowText(std::string s) : data_(new Data{std::move(s), 1}) {}
    CowText(const CowText& o) : data_(o.data_) { ++data_->refs; }   // cheap: share
    CowText& operator=(const CowText& o) {
        ++o.data_->refs;      // guard against self-assignment: bump before release
        release();
        data_ = o.data_;
        return *this;
    }
    ~CowText() { release(); }

    const std::string& read() const { return data_->value; }
    int         share_count() const { return data_->refs; }
    const void* buffer()      const { return data_; }

    // Writing detaches: if the buffer is shared, make a private copy first.
    void write(std::string s) {
        if (data_->refs > 1) {
            --data_->refs;                          // step off the shared buffer
            data_ = new Data{data_->value, 1};      // ...onto a private copy
        }
        data_->value = std::move(s);
    }
};

int main() {
    CowText a{"hello"};
    CowText b = a;               // copy: shares a's buffer, no deep copy
    std::cout << "after copy, share_count = " << a.share_count() << '\n';   // 2
    std::cout << "same buffer? " << (a.buffer() == b.buffer()) << '\n';     // 1

    b.write("world");            // write detaches b onto its own buffer
    std::cout << "after b.write, a = " << a.read() << ", b = " << b.read() << '\n';  // hello, world
    std::cout << "share_count = " << a.share_count()
              << ", same buffer? " << (a.buffer() == b.buffer()) << '\n';   // 1, 0
    return 0;
}
