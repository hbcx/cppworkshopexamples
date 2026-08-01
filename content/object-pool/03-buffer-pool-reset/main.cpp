#include <iostream>
#include <memory>
#include <string>
#include <vector>

// A reusable scratch buffer. reset() returns it to a clean state; clearing the
// string keeps its allocated capacity, so a recycled buffer skips the
// reallocation a freshly built one would pay.
class Buffer {
public:
    void append(const std::string& s) { data_ += s; }
    const std::string& bytes() const { return data_; }
    void reset() { data_.clear(); }
private:
    std::string data_;
};

// A pool that recycles buffers. On return it RESETS the buffer, so the next
// borrower always starts clean -- the reset lives here, once, instead of being
// repeated (and eventually forgotten) at every call site.
class BufferPool {
    struct Return {
        BufferPool* pool;
        void operator()(Buffer* b) const { pool->release(b); }
    };

public:
    using Handle = std::unique_ptr<Buffer, Return>;

    Handle acquire() {
        Buffer* b;
        if (!free_.empty()) {
            b = free_.back();
            free_.pop_back();
        } else {
            owned_.push_back(std::make_unique<Buffer>());
            b = owned_.back().get();
        }
        return Handle(b, Return{this});
    }
    std::size_t created() const { return owned_.size(); }

private:
    void release(Buffer* b) {
        b->reset();               // scrub before it goes back on the free list
        free_.push_back(b);
    }
    std::vector<std::unique_ptr<Buffer>> owned_;
    std::vector<Buffer*> free_;
};

int main() {
    BufferPool pool;
    const char* packets[] = {"GET /", "POST /login", "PING"};

    // A hot loop: each iteration borrows a buffer, fills it, and returns it at
    // the end of the scope. The same buffer serves every packet, and because
    // release() reset it, no packet sees the previous one's bytes.
    for (const char* p : packets) {
        BufferPool::Handle buf = pool.acquire();
        buf->append("frame:");
        buf->append(p);
        std::cout << "processed [" << buf->bytes() << "]\n";
    }   // buf returns here, reset to empty

    std::cout << "buffers ever constructed: " << pool.created() << '\n';
    return 0;
}
