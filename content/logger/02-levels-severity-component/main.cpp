#include "log.hpp"

#include <chrono>
#include <iostream>
#include <sstream>

int main() {
    std::ostringstream file;
    // Deterministic clock so the demo output is reproducible; real code omits it.
    using namespace std::chrono;
    Logger logger{StepClock{sys_days{year{2026} / 8 / 21} + 14h + 5min, 250ms}};
    logger.add_sink(std::make_shared<StreamSink>(file));

    // Component given explicitly as a template argument, with formatted values:
    log<Level::Info, Severity::Minor, Component::Network>(logger, "listening on port {}", 8080);
    log<Level::Error, Severity::Critical, Component::Storage>(logger, "disk {} at {}%", "sda1", 100);

    // Debug is below min_level (Info), so the compiler removes this call: the
    // record is never built and nothing is emitted. (A macro form is needed to
    // also skip evaluating the arguments; here they are literals.)
    log<Level::Debug, Severity::Minor>(logger, "cache size {}", 4096);

    // Component defaults to General when you do not pass one. To bind a component
    // once per file so callers never repeat it -- log_storage(...) instead of
    // log<..., Component::Storage>(...) -- see the per-component wrappers in
    // example 04; they keep the correct call site because the location is
    // captured where the format argument is built, not inside the wrapper.
    log<Level::Info, Severity::Major>(logger, "startup complete");

    logger.flush();
    std::cout << file.str();
    return 0;
}
