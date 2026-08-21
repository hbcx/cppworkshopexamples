#include "logger.hpp"

#include "network.hpp"
#include "storage.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>

int main() {
    // Destinations. In real code these StreamSinks would wrap std::ofstreams --
    // a shared "all.log", a "network.log" and a "storage.log", and an alert
    // channel; here they are in-memory streams so the demo is self-contained.
    std::ostringstream all, networkLog, storageLog, alerts;

    RoutingLogger::Config config;
    config.global = {std::make_shared<StreamSink>(all)};
    config.byComponent[Component::Network] = {std::make_shared<StreamSink>(networkLog)};
    config.byComponent[Component::Storage] = {std::make_shared<StreamSink>(storageLog)};
    config.critical = {std::make_shared<StreamSink>(alerts)};

    // The application owns the logger and installs it as the global. A single
    // producer plus a deterministic clock make the order, sequence numbers and
    // timestamps reproducible.
    using namespace std::chrono;
    RoutingLogger logger(64, std::move(config),
                         StepClock{sys_days{year{2026} / 8 / 21} + 14h + 5min, 250ms});
    set_global_logger(logger);

    // The subsystems log from their own files, with no logger passed in. Each
    // record's origin (storage.cpp / network.cpp) comes from source_location.
    storage::save("blob-42", 8192);
    network::connect("10.0.0.9", 443);

    logger.shutdown(); // drain and join before reading the buffers

    std::cout << "=== all (global) ===\n" << all.str();
    std::cout << "=== network.log ===\n" << networkLog.str();
    std::cout << "=== storage.log ===\n" << storageLog.str();
    std::cout << "=== alerts (critical only) ===\n" << alerts.str();
    return 0;
}
