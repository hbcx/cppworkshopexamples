// The pipeline flavour: middleware that wraps the next handler (the onion).
//
// The other shape of the pattern: instead of exactly one handler processing the
// request, EVERY middleware runs and wraps the next one, doing work before and
// after it calls next(). Control flows inward through the layers to a core
// handler and back out -- the "onion" model of web frameworks. A middleware can
// also short-circuit by NOT calling next(), which is how auth rejects a request
// before it ever reaches the core.

#include <iostream>
#include <functional>
#include <vector>
#include <string>

using Next = std::function<void()>;
using Middleware = std::function<void(const std::string& req, const Next&)>;

class Pipeline {
    std::vector<Middleware> mws_;
    std::function<void(const std::string&)> core_;

    void run(std::size_t i, const std::string& req) {
        if (i < mws_.size())
            mws_[i](req, [this, i, &req]() { run(i + 1, req); }); // next() = the rest
        else
            core_(req);
    }

public:
    Pipeline(std::vector<Middleware> mws, std::function<void(const std::string&)> core)
        : mws_(std::move(mws)), core_(std::move(core)) {}

    void handle(const std::string& req) { run(0, req); }
};

int main() {
    Pipeline pipe(
        {
            [](const std::string&, const Next& next) {          // logging
                std::cout << "[log] begin\n";
                next();
                std::cout << "[log] end\n";
            },
            [](const std::string& req, const Next& next) {      // auth: may short-circuit
                if (req == "secret") {
                    std::cout << "[auth] deny 403\n";
                    return;                                      // does NOT call next()
                }
                std::cout << "[auth] allow\n";
                next();
            },
        },
        [](const std::string& req) {                            // the core handler
            std::cout << "[core] 200 handling " << req << "\n";
        });

    std::cout << "--- request: dashboard ---\n";
    pipe.handle("dashboard");
    std::cout << "--- request: secret ---\n";
    pipe.handle("secret");
    return 0;
}
