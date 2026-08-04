// Anti-pattern: a next_ link that outlives the handler it points to.
//
// Handlers are usually linked by raw pointers, which own nothing -- so if a
// handler is destroyed while an earlier handler's next_ still points at it,
// following that link is use-after-free. This bites when handlers are locals in
// a scope that ends, or are destroyed in the wrong order. The dangling case is
// described in the comment (undefined behaviour, not run). The fix shown here is
// to make the chain OWN its handlers with unique_ptr, so a link can never
// outlive its target -- destroying the head tears down the whole chain in order.
//
// ---- BAD (undefined behaviour -- described, NOT run) ----
//
//   HandlerRaw head;
//   {
//       ConcreteHandler temp;      // a local handler
//       head.setNext(&temp);       // head->next_ now points at temp
//   }                              // temp destroyed here
//   head.handle(req);              // follows next_ into freed 'temp' -> UAF
//
// The link is a non-owning pointer, so nothing ties temp's lifetime to head's;
// when temp dies first, head is left with a dangling next_.

#include <iostream>
#include <memory>

// ---- GOOD: the chain owns its successors, so links cannot dangle ----
class Handler {
    std::unique_ptr<Handler> next_; // owns the rest of the chain
public:
    virtual ~Handler() = default;
    void setNext(std::unique_ptr<Handler> n) { next_ = std::move(n); }
    void handle(int code) {
        if (accepts(code)) { process(code); return; }
        if (next_) next_->handle(code);
        else std::cout << "no handler for " << code << "\n";
    }
protected:
    virtual bool accepts(int) const = 0;
    virtual void process(int) const = 0;
};

class Billing : public Handler {
    bool accepts(int c) const override { return c == 1; }
    void process(int) const override { std::cout << "billing handled\n"; }
};
class Tech : public Handler {
    bool accepts(int c) const override { return c == 2; }
    void process(int) const override { std::cout << "tech handled\n"; }
};

int main() {
    // Build the chain tail-first so each handler owns the next.
    std::unique_ptr<Handler> tech(new Tech());
    std::unique_ptr<Handler> billing(new Billing());
    billing->setNext(std::move(tech));
    // 'billing' now owns the whole chain; destroying it frees Tech too, in order.

    billing->handle(1);
    billing->handle(2);
    billing->handle(9);
    return 0;
}
