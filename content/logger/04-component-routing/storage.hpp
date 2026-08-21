#pragma once
// A storage subsystem, in its own translation unit. It logs through log_storage,
// so its records are tagged Component::Storage and -- via source_location --
// show that they came from storage.cpp.
namespace storage {
void save(const char* name, int bytes);
}
