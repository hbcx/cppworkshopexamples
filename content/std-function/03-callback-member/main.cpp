#include <functional>
#include <iostream>
#include <vector>
#include <string>

// A button that lets callers register handlers to run when it is clicked. The
// handler type is a stored callable, so the button does not care what kind of
// callable it is -- a lambda, a free function, anything with the signature.
class Button {
public:
    using Handler = std::function<void(const std::string&)>;

    void onClick(Handler h) { handlers_.push_back(std::move(h)); }

    void click(const std::string& who) const {
        for (const auto& h : handlers_) h(who);   // fire every registered handler
    }
private:
    std::vector<Handler> handlers_;
};

int main() {
    Button save;
    int clicks = 0;

    save.onClick([](const std::string& who) {
        std::cout << who << " pressed Save\n";
    });
    save.onClick([&clicks](const std::string&) {   // a stateful handler
        ++clicks;
    });

    save.click("Ada");
    save.click("Grace");
    std::cout << "handled clicks = " << clicks << "\n";
    return 0;
}
