#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <list>
#include <new>

// Count every global allocation, to measure how many the intrusive list removes.
static int g_allocs = 0;
void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept              { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void operator delete[](void* p) noexcept              { std::free(p); }
void operator delete[](void* p, std::size_t) noexcept { std::free(p); }

struct Node {
    int   v    = 0;
    Node* next = nullptr;
    Node* prev = nullptr;
};

class IntrusiveList {
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
public:
    void push_back(Node& n) {
        n.prev = tail_; n.next = nullptr;
        if (tail_) tail_->next = &n; else head_ = &n;
        tail_ = &n;
    }
    std::size_t size() const {
        std::size_t k = 0;
        for (Node* p = head_; p; p = p->next) ++k;
        return k;
    }
};

int main() {
    constexpr int N = 100;
    Node storage[N];                 // the elements already exist, on the stack
    for (int i = 0; i < N; ++i) storage[i].v = i;

    g_allocs = 0;
    {
        IntrusiveList list;
        for (int i = 0; i < N; ++i) list.push_back(storage[i]);
        if (list.size() != N) return 1;
    }
    int intrusive = g_allocs;

    g_allocs = 0;
    {
        std::list<int> stdlist;
        for (int i = 0; i < N; ++i) stdlist.push_back(i);
        if (stdlist.size() != N) return 1;
    }
    int stdlist = g_allocs;

    std::cout << "intrusive list, " << N << " elements -> allocations: " << intrusive << '\n';  // 0
    std::cout << "std::list, "      << N << " elements -> allocations: " << stdlist  << '\n';    // 100
    std::cout << "cost: the element carries the hook (sizeof(Node) = "
              << sizeof(Node) << " bytes) and is owned elsewhere\n";
    return 0;
}
