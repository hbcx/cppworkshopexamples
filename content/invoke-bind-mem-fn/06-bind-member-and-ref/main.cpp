#include <functional>
#include <iostream>
#include <string>

struct Logger {
    std::string prefix;
    int count;
    void log(const std::string& msg) {
        ++count;
        std::cout << prefix << msg << "\n";
    }
};

void applyTwice(const std::function<void()>& f) { f(); f(); }

int main() {
    using namespace std::placeholders;
    Logger lg{"[log] ", 0};

    // Binding a member function: the object goes where "this" would, as the
    // argument right after the pointer-to-member. Pass &lg so bind stores a
    // pointer to the real object -- the classic callback before lambdas existed.
    auto logHi = std::bind(&Logger::log, &lg, "hi");
    logHi();
    logHi();
    std::cout << "count after member bind: " << lg.count << "\n";   // 2

    // bind COPIES its bound arguments by default. Bind the object by value and
    // the calls act on that copy, not your variable:
    Logger byVal{"[copy] ", 0};
    auto onCopy = std::bind(&Logger::log, byVal, _1);   // byVal is COPIED into the bind
    onCopy("x");
    std::cout << "original byVal.count: " << byVal.count << "\n";   // 0 -- the copy was logged

    // Wrap the object in std::ref to bind a REFERENCE, so calls hit the original
    // (safe only while it outlives the bound object). std::ref has its own chapter.
    Logger byRef{"[ref] ", 0};
    auto onRef = std::bind(&Logger::log, std::ref(byRef), _1);
    onRef("y");
    std::cout << "original byRef.count: " << byRef.count << "\n";   // 1 -- the original

    // A bound thunk drops straight into a std::function, so bind + function is a
    // complete stored callback.
    applyTwice(std::bind(&Logger::log, &lg, "tick"));
    return 0;
}
