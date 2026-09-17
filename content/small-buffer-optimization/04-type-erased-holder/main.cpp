#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

// A std::function-like holder for an int(int) callable, with the small-buffer
// optimization: a small callable is stored inline, a large one on the heap. This
// is how std::function avoids an allocation for a small lambda.
class Function {
    static constexpr std::size_t kInline = 24;
    // A hand-written vtable: how to call the erased callable, and how to destroy
    // it. Non-capturing lambdas convert to these plain function pointers.
    struct VTable {
        int  (*invoke)(void* obj, int arg);
        void (*destroy)(void* obj);
    };
    alignas(std::max_align_t) unsigned char buffer_[kInline];
    void*         obj_  = nullptr;   // points at buffer_ (inline) or the heap
    const VTable* vt_   = nullptr;
    bool          heap_ = false;

    template <class F>
    static const VTable* vtable_for() {
        static const VTable vt{
            [](void* o, int a) { return (*static_cast<F*>(o))(a); },
            [](void* o)        { static_cast<F*>(o)->~F(); },
        };
        return &vt;
    }
public:
    template <class F>
    Function(F f) {
        vt_ = vtable_for<F>();
        // if constexpr so the branch that does not apply is discarded at compile
        // time: without it, the inline placement-new would be compiled even for
        // an F too big for the buffer, which the compiler rightly rejects.
        if constexpr (sizeof(F) <= kInline && alignof(F) <= alignof(std::max_align_t)) {
            obj_  = ::new (buffer_) F(std::move(f));   // inline: no allocation
            heap_ = false;
        } else {
            obj_  = new F(std::move(f));               // too big: heap
            heap_ = true;
        }
    }
    ~Function() {
        if (obj_) {
            vt_->destroy(obj_);                 // run the callable's destructor
            if (heap_) ::operator delete(obj_); // and free the heap block if any
        }
    }
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;

    int  operator()(int arg) const { return vt_->invoke(obj_, arg); }
    bool on_heap()           const { return heap_; }
};

int main() {
    int bonus = 10;
    Function f1{[bonus](int x) { return x + bonus; }};   // tiny capture -> inline
    std::cout << "f1(5) = " << f1(5) << ", on heap? " << f1.on_heap() << '\n';   // 15, 0

    // A large capture does not fit the 24-byte inline buffer -> heap.
    long long a = 1, b = 2, c = 3, d = 4, e = 5;         // 40 bytes of capture
    Function f2{[a, b, c, d, e](int x) { return x + static_cast<int>(a + b + c + d + e); }};
    std::cout << "f2(5) = " << f2(5) << ", on heap? " << f2.on_heap() << '\n';   // 20, 1
    return 0;
}
