#include <iostream>
#include <string>

// Subsystem type -- an implementation detail of the HTTP engine.
struct RawResponse {
    int status;
    std::string body;
};

class HttpEngine {
public:
    RawResponse get(const std::string& url) const {
        return RawResponse{200, "payload for " + url};
    }
};

// ANTI-PATTERN: the facade returns a RawResponse -- a SUBSYSTEM type. The client
// must now know that type, depend on its header, and interpret its fields, so it
// is coupled to the subsystem exactly as if there were no facade. Replace the
// engine with one whose response type differs and every caller breaks.
class GatewayBad {
public:
    RawResponse fetch(const std::string& url) { return engine_.get(url); }
private:
    HttpEngine engine_;
};

// FIX: the facade exposes its OWN simple type. The subsystem type stays behind
// the facade; the client depends only on Result, so the engine can be swapped
// without touching callers.
struct Result {
    bool ok;
    std::string text;
};

class GatewayGood {
public:
    Result fetch(const std::string& url) {
        const RawResponse r = engine_.get(url);
        return Result{r.status == 200, r.body};
    }
private:
    HttpEngine engine_;
};

int main() {
    GatewayBad bad;
    const RawResponse raw = bad.fetch("/a");        // client handles a subsystem type
    std::cout << "bad:  status=" << raw.status << " body=" << raw.body << "\n";

    GatewayGood good;
    const Result res = good.fetch("/a");            // client handles only the facade's type
    std::cout << "good: ok=" << (res.ok ? "true" : "false") << " text=" << res.text << "\n";
    return 0;
}
