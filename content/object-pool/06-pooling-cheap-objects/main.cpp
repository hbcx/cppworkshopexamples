#include <iostream>
#include <memory>
#include <vector>

// A cheap, trivially-constructed value: no allocation, no work.
struct Point { int x, y; };

// Anti-pattern: a pool for Point. Every use now needs a handle, a free list,
// and a return path -- indirection and lifetime management bolted onto an
// object whose construction was two integer stores.
class PointPool {
    struct Return { PointPool* pool; void operator()(Point* p) const { pool->release(p); } };
public:
    using Handle = std::unique_ptr<Point, Return>;
    Handle acquire() {
        Point* p;
        if (!free_.empty()) { p = free_.back(); free_.pop_back(); }
        else { owned_.push_back(std::make_unique<Point>()); p = owned_.back().get(); }
        return Handle(p, Return{this});
    }
private:
    void release(Point* p) { p->x = 0; p->y = 0; free_.push_back(p); }
    std::vector<std::unique_ptr<Point>> owned_;
    std::vector<Point*> free_;
};

int main() {
    // Pooled: heap storage, handles, acquire/release routing -- all to avoid
    // constructing a two-int struct.
    PointPool pool;
    int sum = 0;
    for (int i = 1; i <= 3; ++i) {
        PointPool::Handle p = pool.acquire();
        p->x = i;
        p->y = i * 10;
        sum += p->x + p->y;
    }
    std::cout << "pooled sum = " << sum << '\n';

    // Fix: just make the Point. It lives on the stack, there is no heap, no
    // free list, no lifetime to manage -- and it is faster.
    sum = 0;
    for (int i = 1; i <= 3; ++i) {
        Point p{i, i * 10};
        sum += p.x + p.y;
    }
    std::cout << "stack sum  = " << sum << '\n';
    return 0;
}
