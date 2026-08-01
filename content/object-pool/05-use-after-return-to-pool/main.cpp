#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Session {
    int id;
    std::string user;
    explicit Session(int id_) : id(id_) {}
};

class SessionPool {
    struct Return { SessionPool* pool; void operator()(Session* s) const { pool->release(s); } };
public:
    using Handle = std::unique_ptr<Session, Return>;
    Handle acquire() {
        Session* s;
        if (!free_.empty()) { s = free_.back(); free_.pop_back(); }
        else { owned_.push_back(std::make_unique<Session>(nextId_++)); s = owned_.back().get(); }
        return Handle(s, Return{this});
    }
private:
    void release(Session* s) { free_.push_back(s); }
    std::vector<std::unique_ptr<Session>> owned_;
    std::vector<Session*> free_;
    int nextId_ = 1;
};

int main() {
    SessionPool pool;

    // Anti-pattern: stash a raw pointer that outlives the handle owning the
    // loan. Once the handle returns the object, this pointer aliases whatever
    // the pool hands out next -- it is NOT yours anymore.
    Session* stale = nullptr;
    {
        SessionPool::Handle h = pool.acquire();
        h->user = "alice";
        stale = h.get();                 // pointer escapes the handle's lifetime
    }   // h returns the Session to the pool here

    // A different borrower now legitimately gets that very same object back.
    SessionPool::Handle other = pool.acquire();
    other->user = "bob";

    // Both refer to one object: the write through `other` is visible through
    // `stale`. Same Session id, silently shared.
    std::cout << "other sees user=" << other->user
              << " (Session #" << other->id << ")\n";
    std::cout << "stale sees user=" << stale->user
              << " (Session #" << stale->id << ")\n";

    // Fix: never let a raw pointer to a pooled object escape its loan. Hold the
    // object only through the move-only Handle, and only while you use it.
    return 0;
}
