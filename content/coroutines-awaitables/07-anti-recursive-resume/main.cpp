#include <coroutine>
#include <iostream>
#include <vector>

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> handle;
    explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
    Task(Task&& o) noexcept : handle(o.handle) { o.handle = nullptr; }
    Task(const Task&) = delete;
    ~Task() { if (handle) handle.destroy(); }
};

// We measure how deeply await_suspend calls nest on the real C++ call stack.
long g_depth = 0;
long g_max_depth = 0;

// ANTI-PATTERN: resume the next coroutine from INSIDE await_suspend. That
// resume() runs the next coroutine's await_suspend, which resumes the next, and
// so on -- every hop is a nested call, so the stack grows with the chain length.
// A long enough chain overflows the stack. (Here we only count the depth.)
struct ResumeInSuspend {
    std::coroutine_handle<> next;
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<>) const noexcept {
        ++g_depth;
        if (g_depth > g_max_depth) g_max_depth = g_depth;
        if (next) next.resume();     // nested resume -> the stack grows here
        --g_depth;
    }
    void await_resume() const noexcept {}
};

// FIX: return the next handle. The runtime resumes it as a tail call, so nothing
// nests -- the depth stays flat no matter how long the chain is.
struct TransferInSuspend {
    std::coroutine_handle<> next;
    bool await_ready() const noexcept { return false; }
    std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept {
        ++g_depth;
        if (g_depth > g_max_depth) g_max_depth = g_depth;
        std::coroutine_handle<> h = next ? next : std::noop_coroutine();
        --g_depth;
        return h;                    // symmetric transfer -> no nesting
    }
    void await_resume() const noexcept {}
};

std::vector<std::coroutine_handle<>> g_chain;

Task node(std::size_t i, bool useTransfer) {
    std::coroutine_handle<> next =
        (i + 1 < g_chain.size()) ? g_chain[i + 1] : std::noop_coroutine();
    if (useTransfer)
        co_await TransferInSuspend{next};
    else
        co_await ResumeInSuspend{next};
}

long run(bool useTransfer) {
    g_depth = 0;
    g_max_depth = 0;
    g_chain.clear();
    const std::size_t N = 6;
    std::vector<Task> tasks;
    for (std::size_t i = 0; i < N; ++i)
        tasks.push_back(node(i, useTransfer));   // lazy: bodies have not run yet
    for (auto& t : tasks)
        g_chain.push_back(t.handle);             // now every "next" is known
    tasks[0].handle.resume();                    // kick off the chain
    return g_max_depth;
}

int main() {
    std::cout << "recursive resume (anti):  max nesting depth = " << run(false)
              << "  (grows with the chain -> stack overflow risk)\n";
    std::cout << "symmetric transfer (fix): max nesting depth = " << run(true)
              << "  (flat, no matter how long the chain)\n";
    return 0;
}
