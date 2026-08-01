#include <iostream>
#include <memory>
#include <string>
#include <vector>

// A message buffer borrowed from a pool.
class Message {
public:
    void set(const std::string& s) { body_ = s; }
    const std::string& body() const { return body_; }
    void reset() { body_.clear(); }
private:
    std::string body_;
};

// Anti-pattern: release() puts the object straight back on the free list
// without clearing it, so the next borrower inherits the previous body.
class LeakyPool {
    struct Return { LeakyPool* pool; void operator()(Message* m) const { pool->release(m); } };
public:
    using Handle = std::unique_ptr<Message, Return>;
    Handle acquire() {
        Message* m;
        if (!free_.empty()) { m = free_.back(); free_.pop_back(); }
        else { owned_.push_back(std::make_unique<Message>()); m = owned_.back().get(); }
        return Handle(m, Return{this});
    }
private:
    void release(Message* m) { free_.push_back(m); }   // BUG: no m->reset()
    std::vector<std::unique_ptr<Message>> owned_;
    std::vector<Message*> free_;
};

// Fix: the pool resets each object on return, so every borrower starts clean.
class CleanPool {
    struct Return { CleanPool* pool; void operator()(Message* m) const { pool->release(m); } };
public:
    using Handle = std::unique_ptr<Message, Return>;
    Handle acquire() {
        Message* m;
        if (!free_.empty()) { m = free_.back(); free_.pop_back(); }
        else { owned_.push_back(std::make_unique<Message>()); m = owned_.back().get(); }
        return Handle(m, Return{this});
    }
private:
    void release(Message* m) { m->reset(); free_.push_back(m); }   // fixed
    std::vector<std::unique_ptr<Message>> owned_;
    std::vector<Message*> free_;
};

int main() {
    // Leaky pool: borrower #1 leaves a secret, borrower #2 reads it back.
    {
        LeakyPool pool;
        { auto m = pool.acquire(); m->set("user-A secret"); }   // returned, not cleared
        auto m = pool.acquire();                                 // same object handed back
        std::cout << "leaky pool -> new borrower sees: [" << m->body() << "]\n";
    }

    // Clean pool: reset on release, so borrower #2 sees an empty message.
    {
        CleanPool pool;
        { auto m = pool.acquire(); m->set("user-A secret"); }
        auto m = pool.acquire();
        std::cout << "clean pool -> new borrower sees: [" << m->body() << "]\n";
    }
    return 0;
}
