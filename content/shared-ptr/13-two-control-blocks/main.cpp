#include <iostream>
#include <memory>

struct Tracked {
    int id;
    explicit Tracked(int i) : id(i) { std::cout << "Tracked " << id << " ctor\n"; }
    ~Tracked() { std::cout << "Tracked " << id << " dtor\n"; }
};

int main() {
    // --- the bug (NOT executed: it double-frees, which is UB) ---
    // Tracked* raw = new Tracked(1);
    // std::shared_ptr<Tracked> a(raw);
    // std::shared_ptr<Tracked> b(raw);   // SEPARATE control block, count 1 each
    // // a's count hits 0 and deletes raw; then b's count hits 0 and deletes it
    // // again -> double free. Each shared_ptr thinks it is the sole owner.

    // --- the fix: make ONE shared_ptr, then COPY it to share the same block ---
    std::shared_ptr<Tracked> a(new Tracked(1));
    std::shared_ptr<Tracked> b = a;    // same control block, one shared count
    std::cout << "shared correctly, use_count = " << a.use_count() << '\n';   // 2

    // --- best: make_shared never exposes a raw pointer to wrap twice ---
    auto c = std::make_shared<Tracked>(2);
    std::cout << "c use_count = " << c.use_count() << '\n';   // 1
    return 0;   // Tracked 1 deleted once (by a and b), Tracked 2 once (by c)
}
