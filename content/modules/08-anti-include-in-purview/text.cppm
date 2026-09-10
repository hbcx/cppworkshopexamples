// FIX: legacy headers go in the global module fragment, before export module.
module;
#include <string>
#include <cctype>

export module text;

// ANTI-PATTERN (described): a #include AFTER export module -- in the module
// purview -- is wrong. The header's declarations would attach to your module, and
// standard headers are not meant to be included there at all:
//
//     export module text;
//     #include <string>     // ill-formed / attaches <string> to module text
//
// Textual includes belong in the global module fragment above.

export std::string shout(std::string s) {
    for (char& c : s)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}
