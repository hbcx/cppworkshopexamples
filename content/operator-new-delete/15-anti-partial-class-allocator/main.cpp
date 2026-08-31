#include <iostream>
#include <new>
#include <cstddef>

// Only the SCALAR class operator new/delete are defined. Array allocations
// (new Pooled[n]) look up operator new[], do not find one on the class, and
// fall back to the GLOBAL array new -- silently bypassing this pool.
struct Pooled {
    static int fromPool;
    int x;
    static void* operator new(std::size_t n) {
        ++fromPool;
        return ::operator new(n);
    }
    static void operator delete(void* p) noexcept {
        --fromPool;
        ::operator delete(p);
    }
};
int Pooled::fromPool = 0;

int main() {
    Pooled* one = new Pooled;      // class operator new -> counted
    std::cout << "after single new: fromPool = " << Pooled::fromPool << "\n";

    Pooled* arr = new Pooled[3];   // GLOBAL operator new[] -> NOT counted
    std::cout << "after array new:  fromPool = " << Pooled::fromPool
              << " (array bypassed the pool)\n";

    delete[] arr;                  // global operator delete[]
    delete one;                    // class operator delete
    std::cout << "after cleanup:    fromPool = " << Pooled::fromPool << "\n";
}
