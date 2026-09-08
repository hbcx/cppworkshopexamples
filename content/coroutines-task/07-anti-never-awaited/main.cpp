#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

// A counter so we can see whether the task body ever executed.
long g_ran = 0;

template <typename T>
struct Task {
    struct promise_type {
        T result{};
        std::exception_ptr error{};
        std::coroutine_handle<> continuation{};

        Task get_return_object() {
            return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }   // lazy

        struct FinalAwaiter {
            bool await_ready() const noexcept { return false; }
            std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) const noexcept {
                std::coroutine_handle<> cont = h.promise().continuation;
                return cont ? cont : std::noop_coroutine();
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

    T get() {
        if (handle.promise().error)
            std::rethrow_exception(handle.promise().error);
        return std::move(handle.promise().result);
    }
};

Task<int> work() {
    ++g_ran;                       // the side effect we care about
    std::cout << "  work() body ran\n";
    co_return 1;
}

int main() {
    // ANTI-PATTERN: a lazy task does nothing until it is awaited or driven.
    // Creating one and dropping it runs no code -- a common surprise when someone
    // expects the call itself to do the work (as an eager std::async would).
    g_ran = 0;
    {
        Task<int> t = work();      // parked at initial_suspend, body NOT run
        (void)t;                   // ... and we forget to await or drive it ...
    }                              // ~Task destroys the never-started frame
    std::cout << "not driven: body ran " << g_ran << " times\n";

    // FIX: drive it -- here by hand; normally you co_await it or sync_wait it.
    g_ran = 0;
    {
        Task<int> t = work();
        t.handle.resume();
        std::cout << "driven: body ran " << g_ran << " times, result " << t.get() << "\n";
    }
    return 0;
}
