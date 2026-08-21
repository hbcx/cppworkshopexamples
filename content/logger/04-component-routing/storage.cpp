#include "storage.hpp"

#include "logger.hpp"

namespace storage {

void save(const char* name, int bytes) {
    // No logger object is passed in: log_storage uses the installed global one
    // and binds Component::Storage. The call site (this file, this function) is
    // captured automatically.
    log_storage<Level::Info, Severity::Minor>("saving {} ({} bytes)", name, bytes);
    if (bytes > 4096)
        log_storage<Level::Error, Severity::Critical>("object {} exceeds the quota", name);
}

} // namespace storage
