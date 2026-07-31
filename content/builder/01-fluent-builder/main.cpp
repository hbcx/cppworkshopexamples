#include <iostream>
#include <string>
#include <utility>

// The product: immutable once built. It has one required field (url) and several
// optional ones, so a single constructor would be a long positional list.
class HttpRequest {
public:
    class Builder;   // the only way to make one

    const std::string& url() const { return url_; }
    const std::string& method() const { return method_; }
    int timeout() const { return timeout_; }
    bool followRedirects() const { return followRedirects_; }
    const std::string& body() const { return body_; }

    void print() const {
        std::cout << method_ << ' ' << url_
                  << "  timeout=" << timeout_ << 's'
                  << " redirects=" << (followRedirects_ ? "on" : "off")
                  << " body=\"" << body_ << "\"\n";
    }

private:
    // Private: only the Builder (a friend) constructs a finished request.
    HttpRequest(std::string url, std::string method, int timeout,
                bool followRedirects, std::string body)
        : url_(std::move(url)), method_(std::move(method)), timeout_(timeout),
          followRedirects_(followRedirects), body_(std::move(body)) {}
    friend class Builder;

    std::string url_;
    std::string method_;
    int timeout_;
    bool followRedirects_;
    std::string body_;
};

// The builder: one named setter per field, each returning *this so calls chain.
// Defaults live here once, in the member initializers.
class HttpRequest::Builder {
public:
    explicit Builder(std::string url) : url_(std::move(url)) {}

    Builder& method(std::string m) { method_ = std::move(m); return *this; }
    Builder& timeout(int seconds) { timeout_ = seconds; return *this; }
    Builder& followRedirects(bool on) { followRedirects_ = on; return *this; }
    Builder& body(std::string b) { body_ = std::move(b); return *this; }

    HttpRequest build() const {
        return HttpRequest(url_, method_, timeout_, followRedirects_, body_);
    }

private:
    std::string url_;
    std::string method_ = "GET";
    int timeout_ = 30;
    bool followRedirects_ = true;
    std::string body_;
};

int main() {
    // All defaults but the required url.
    HttpRequest get = HttpRequest::Builder("https://example.com/data").build();
    get.print();

    // Only the fields that differ are named -- no positional guessing.
    HttpRequest post = HttpRequest::Builder("https://api.example.com/users")
                           .method("POST")
                           .timeout(10)
                           .followRedirects(false)
                           .body("{name:Ada}")
                           .build();
    post.print();
    return 0;
}
