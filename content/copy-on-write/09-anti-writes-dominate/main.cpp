#include <iostream>
#include <string>
#include <utility>

static int g_deepCopies = 0;

struct Buffer {
    std::string bytes;
    explicit Buffer(std::string b) : bytes(std::move(b)) {}
    Buffer(const Buffer& o) : bytes(o.bytes) { ++g_deepCopies; }
    Buffer& operator=(const Buffer&) = default;
};

// Copy-on-write value.
class CowValue {
    struct Data { Buffer buf; int refs = 1; };
    Data* data_;
    void release() { if (--data_->refs == 0) delete data_; }
public:
    explicit CowValue(std::string b) : data_(new Data{Buffer{std::move(b)}, 1}) {}
    CowValue(const CowValue& o) : data_(o.data_) { ++data_->refs; }
    CowValue& operator=(const CowValue&) = delete;
    ~CowValue() { release(); }
    void write(std::string b) {
        if (data_->refs > 1) { --data_->refs; data_ = new Data{Buffer{data_->buf}, 1}; }  // detach
        data_->buf.bytes = std::move(b);
    }
};

// Plain value: every copy deep-copies.
class PlainValue {
    Buffer buf_;
public:
    explicit PlainValue(std::string b) : buf_(std::move(b)) {}
    PlainValue(const PlainValue&) = default;
    void write(std::string b) { buf_.bytes = std::move(b); }
};

int main() {
    constexpr int N = 100;

    // Workload: copy, then immediately write. Sharing never lasts.
    CowValue cowBase{"payload"};
    g_deepCopies = 0;
    for (int i = 0; i < N; ++i) {
        CowValue copy = cowBase;   // share (0 deep copies)...
        copy.write("x");           // ...but write at once -> detach (1 deep copy)
    }
    int cow = g_deepCopies;

    PlainValue plainBase{"payload"};
    g_deepCopies = 0;
    for (int i = 0; i < N; ++i) {
        PlainValue copy = plainBase;   // deep copy (1)
        copy.write("x");               // in place
    }
    int plain = g_deepCopies;

    std::cout << "COW deep copies:   " << cow << '\n';     // 100
    std::cout << "plain deep copies: " << plain << '\n';   // 100
    std::cout << "same deep copies -- but COW also paid for a refcounted control\n"
                 "block and a bump/release on every copy. When writes follow copies,\n"
                 "copy-on-write is pure overhead; use a plain value type.\n";
    return 0;
}
