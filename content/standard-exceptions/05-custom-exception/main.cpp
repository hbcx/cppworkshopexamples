// A custom exception: derive from std::runtime_error (or logic_error) so it fits
// the standard hierarchy -- existing catch (const std::exception&) and
// catch (const std::runtime_error&) handlers still catch it -- while carrying
// extra, structured data the handler can use beyond the text message.
#include <iostream>
#include <stdexcept>
#include <string>

class HttpError : public std::runtime_error {
public:
    HttpError(int status, const std::string& url)
        // Build the message once and hand it to the base, which stores a copy,
        // so what() stays valid without us managing any buffer.
        : std::runtime_error("HTTP " + std::to_string(status) + " for " + url),
          status_(status), url_(url) {}

    int status() const noexcept { return status_; }
    const std::string& url() const noexcept { return url_; }

private:
    int status_;
    std::string url_;
};

int main() {
    // Caught by the base -- a generic handler still works...
    try {
        throw HttpError(404, "/api/widget/42");
    }
    catch (const std::runtime_error& e) {
        std::cout << "as runtime_error: " << e.what() << '\n';
    }

    // ...or caught as the specific type to reach the extra data.
    try {
        throw HttpError(503, "/api/order");
    }
    catch (const HttpError& e) {
        std::cout << "status=" << e.status() << " url=" << e.url() << '\n';
        std::cout << "retryable=" << std::boolalpha << (e.status() >= 500) << '\n';
    }
    return 0;
}
