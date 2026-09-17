#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

// Why std::string is NOT copy-on-write since C++11:
//  1. The shared reference count is touched from every thread that copies or
//     destroys a value, so it MUST be atomic -- a plain int races and can double
//     free or leak. This version uses std::atomic and is TSan-clean.
//  2. Even with an atomic count, a non-const operator[] would hand out a
//     reference into the shared buffer; if another owner then writes (detaches),
//     that reference dangles. The count cannot fix that -- it is the deeper
//     reason copy-on-write was banned for std::string. (See the anti-patterns.)
class CowText {
    struct Data {
        std::string value;
        std::atomic<int> refs{1};
    };
    Data* data_;
    void release() { if (data_->refs.fetch_sub(1) == 1) delete data_; }
public:
    explicit CowText(std::string s) : data_(new Data{std::move(s)}) {}
    CowText(const CowText& o) : data_(o.data_) { data_->refs.fetch_add(1); }
    CowText& operator=(const CowText&) = delete;   // keep the demo focused
    ~CowText() { release(); }
    const std::string& read() const { return data_->value; }
};

int main() {
    CowText original{"shared"};

    // Many threads copy and destroy the value at once. The reference count is hit
    // from all of them simultaneously; only because it is atomic does this not
    // race -- run under ThreadSanitizer to confirm.
    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i) {
        pool.emplace_back([&original] {
            for (int k = 0; k < 1000; ++k) {
                CowText copy = original;   // atomic increment
                (void) copy.read();
            }                              // atomic decrement on scope exit
        });
    }
    for (auto& t : pool) t.join();

    std::cout << "original intact after concurrent copies: "
              << (original.read() == "shared") << '\n';   // 1
    return 0;
}
