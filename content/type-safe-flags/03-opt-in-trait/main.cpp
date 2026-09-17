#include <iostream>
#include <type_traits>

// Opt-in trait: a flag enum turns the operators on by specializing this to true.
// Enums that do not stay plain scoped enums with no bitwise operators.
template <class E>
struct enable_bitmask_operators : std::false_type {};

// The operators, written ONCE as templates. enable_if makes each one exist only
// for enums that opted in, so a plain enum class never accidentally gets bitwise
// operators, and only one definition is maintained for all flag enums.
template <class E, class = std::enable_if_t<enable_bitmask_operators<E>::value>>
constexpr E operator|(E a, E b) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) | static_cast<U>(b));
}
template <class E, class = std::enable_if_t<enable_bitmask_operators<E>::value>>
constexpr E operator&(E a, E b) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(a) & static_cast<U>(b));
}
template <class E, class = std::enable_if_t<enable_bitmask_operators<E>::value>>
constexpr bool has(E set, E flag) {
    return (set & flag) == flag;
}

// Two unrelated flag enums, each opting in with a single line.
enum class FileMode : unsigned { Read = 1u << 0, Write = 1u << 1, Create = 1u << 2 };
enum class LogLevel : unsigned { Info = 1u << 0, Warn  = 1u << 1, Error  = 1u << 2 };

// An enum that does NOT opt in: it never gets the operators.
enum class Color { Red, Green, Blue };

template <> struct enable_bitmask_operators<FileMode> : std::true_type {};
template <> struct enable_bitmask_operators<LogLevel> : std::true_type {};

int main() {
    FileMode fm = FileMode::Read | FileMode::Create;
    std::cout << "FileMode has Create: " << has(fm, FileMode::Create) << '\n';  // 1
    std::cout << "FileMode has Write:  " << has(fm, FileMode::Write) << '\n';   // 0

    LogLevel lv = LogLevel::Warn | LogLevel::Error;
    std::cout << "LogLevel has Error:  " << has(lv, LogLevel::Error) << '\n';   // 1

    // Neither of these compiles, which is the safety:
    //   FileMode::Read | LogLevel::Info;  // different E on each side -> type error
    //   Color::Red | Color::Green;        // Color did not opt in -> no operator
    std::cout << "raw bits: " << static_cast<unsigned>(fm)
              << ", " << static_cast<unsigned>(lv) << '\n';                     // 5, 6
    return 0;
}
