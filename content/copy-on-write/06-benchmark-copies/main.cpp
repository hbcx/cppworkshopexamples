#include <iostream>
#include <string>
#include <utility>
#include <vector>

static int g_deepCopies = 0;

// A payload that is expensive to copy; its copy constructor counts deep copies.
struct Buffer {
    std::string bytes;
    explicit Buffer(std::string b) : bytes(std::move(b)) {}
    Buffer(const Buffer& o) : bytes(o.bytes) { ++g_deepCopies; }
    Buffer& operator=(const Buffer&) = default;
};

// Copy-on-write: copying shares the buffer; only a write copies it.
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

// A plain value: every copy deep-copies the buffer.
class PlainValue {
    Buffer buf_;
public:
    explicit PlainValue(std::string b) : buf_(std::move(b)) {}
    PlainValue(const PlainValue&) = default;   // deep copy (counted)
};

int main() {
    constexpr int N = 100;

    g_deepCopies = 0;
    {
        CowValue base{"payload"};
        std::vector<CowValue> copies;
        copies.reserve(N);
        for (int i = 0; i < N; ++i) copies.push_back(base);   // share, no deep copy
    }
    std::cout << "COW: " << N << " copies -> deep copies: " << g_deepCopies << '\n';   // 0

    g_deepCopies = 0;
    {
        CowValue base{"payload"};
        CowValue c = base;    // shared
        c.write("changed");   // first write after sharing detaches -> ONE deep copy
    }
    std::cout << "COW: one write after sharing -> deep copies: " << g_deepCopies << '\n';   // 1

    g_deepCopies = 0;
    {
        PlainValue base{"payload"};
        std::vector<PlainValue> copies;
        copies.reserve(N);
        for (int i = 0; i < N; ++i) copies.push_back(base);   // deep copy every time
    }
    std::cout << "plain value: " << N << " copies -> deep copies: " << g_deepCopies << '\n';   // 100
    return 0;
}
