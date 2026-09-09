#include "parts.hpp"

// A DIFFERENT who(), also in an unnamed namespace. Same name as main.cpp's, but
// no ODR violation and no linker clash: each has internal linkage.
namespace {
    const char* who() { return "other.cpp"; }
}

std::string fromOther() { return who(); }
