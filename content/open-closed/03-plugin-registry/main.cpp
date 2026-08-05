// Open-Closed with a registry -- the dispatcher is closed, handlers extend it.
//
// A message dispatcher looks each type up in a table and calls its handler.
// Supporting a new message type is registering a new handler; the dispatch code
// never changes. This is how plugin systems and format loaders stay open.

#include <cctype>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

class Dispatcher {
public:
    using Handler = std::function<std::string(const std::string&)>;

    void on(const std::string& type, Handler h) { handlers_[type] = std::move(h); }

    std::string dispatch(const std::string& type, const std::string& payload) const {
        std::unordered_map<std::string, Handler>::const_iterator it = handlers_.find(type);
        if (it == handlers_.end()) return "[no handler for '" + type + "']";
        return it->second(payload);
    }

private:
    std::unordered_map<std::string, Handler> handlers_;
};

int main() {
    Dispatcher d;
    d.on("echo", [](const std::string& p) { return p; });
    d.on("upper", [](const std::string& p) {
        std::string r = p;
        for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return r;
    });

    std::cout << d.dispatch("echo", "hi") << "\n";
    std::cout << d.dispatch("upper", "hi") << "\n";
    std::cout << d.dispatch("shout", "hi") << "\n"; // not registered yet

    // EXTENSION: register a new type later -- Dispatcher is not modified.
    d.on("shout", [](const std::string& p) { return p + "!!!"; });
    std::cout << d.dispatch("shout", "hi") << "\n";
}
