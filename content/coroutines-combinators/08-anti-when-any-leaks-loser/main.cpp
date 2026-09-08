#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

struct Loop {
    std::multimap<long, std::coroutine_handle<>> timers;
    long now = 0;
    void sleep_until(long t, std::coroutine_handle<> h) { timers.emplace(t, h); }
    void cancel(std::coroutine_handle<> h) {
        for (auto it = timers.begin(); it != timers.end(); ) {
            if (it->second == h) it = timers.erase(it);
            else ++it;
        }
    }
    void run() {
        while (!timers.empty()) {
            auto it = timers.begin();
            now = it->first;
            std::coroutine_handle<> h = it->second;
            timers.erase(it);
            h.resume();
        }
    }
};
Loop g_loop;

struct Sleep {
    long ms;
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        g_loop.sleep_until(g_loop.now + ms, h);
    }
    void await_resume() const noexcept {}
};

template <typename T>
struct Task {
    struct promise_type {
        T result{};
        std::exception_ptr error{};
        std::function<void()> on_complete;
        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            void await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                if (h.promise().on_complete) h.promise().on_complete();
            }
            void await_resume() const noexcept {}
        };
        FinalAwaiter final_suspend() noexcept { return {}; }
        void return_value(T value) { result = std::move(value); }
        void unhandled_exception() { error = std::current_exception(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }
    void start(std::function<void()> cb) {
        handle.promise().on_complete = std::move(cb);
        handle.resume();
    }
    int result() const { return handle.promise().result; }
};

// Counts the real work each task does AFTER its wait -- the part we wanted to
// avoid for the losers once a winner is picked.
long g_work = 0;

Task<int> fetch(int id, long ms) {
    co_await Sleep{ms};
    ++g_work;                 // the expensive step that should not run for losers
    co_return id * 10;
}

// The when_any join state is kept in globals, and -- importantly -- the tasks are
// owned by main, not by this awaiter or by race. That is what lets the no-cancel
// path be RUN safely: the loser frames outlive the whole loop, so the loop can
// wake them without touching freed memory.
//
// Why that matters: a coroutine destroys its local variables at co_return, BEFORE
// its final suspend. So if race owned the tasks, race finishing would destroy the
// losers while their wake-ups were still queued, and the loop would then resume
// freed frames -- undefined behaviour, not just wasted work. Owning the losers
// somewhere long-lived turns the bug into the observable "they ran anyway" below;
// a real combinator instead OWNS them and therefore MUST cancel them.
bool g_resolved = false;
int g_value = 0;
std::coroutine_handle<> g_joiner{};
std::vector<Task<int>>* g_tasks = nullptr;   // borrowed; owned by main
bool g_cancel = false;

struct WhenAny {
    bool await_ready() const noexcept { return g_tasks->empty(); }
    void await_suspend(std::coroutine_handle<> h) const {
        g_joiner = h;
        for (std::size_t i = 0; i < g_tasks->size(); ++i) {
            (*g_tasks)[i].start([i] {
                if (g_resolved) return;                 // a faster task already won
                g_resolved = true;
                g_value = (*g_tasks)[i].result();
                if (g_cancel) {
                    for (std::size_t j = 0; j < g_tasks->size(); ++j)
                        if (j != i) g_loop.cancel((*g_tasks)[j].handle);
                }
                g_loop.sleep_until(g_loop.now, g_joiner);
            });
        }
    }
    int await_resume() const { return g_value; }
};

Task<int> race() {
    int v = co_await WhenAny{};      // races the tasks main handed us via g_tasks
    std::cout << "  winner value = " << v << " at t=" << g_loop.now << "\n";
    co_return 0;
}

void runRace(std::vector<Task<int>>& tasks, bool cancelLosers) {
    g_work = 0;
    g_loop.now = 0;
    g_resolved = false;
    g_value = 0;
    g_tasks = &tasks;
    g_cancel = cancelLosers;
    Task<int> p = race();
    p.start([]{});
    g_loop.run();                    // fully drains before returning
}

int main() {
    // ANTI: no cancellation. The two losers stay scheduled; the loop wakes them
    // and their work runs even though the race was already decided.
    std::vector<Task<int>> anti;
    anti.push_back(fetch(1, 30));
    anti.push_back(fetch(2, 10));    // winner
    anti.push_back(fetch(3, 20));
    runRace(anti, false);
    std::cout << "without cancel (anti): work done by " << g_work << " tasks\n\n";

    // FIX: cancel the losers when the winner is chosen -- only its work runs.
    std::vector<Task<int>> fixed;
    fixed.push_back(fetch(1, 30));
    fixed.push_back(fetch(2, 10));
    fixed.push_back(fetch(3, 20));
    runRace(fixed, true);
    std::cout << "with cancel (fix):     work done by " << g_work << " task\n";
    return 0;
}
