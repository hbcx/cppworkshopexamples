#include <iostream>
#include <cstdint>
#include <type_traits>

// Every enum has an underlying integer type. You can name it explicitly, both to
// control the size (for a protocol or a compact field) and to fix the value
// range. std::underlying_type reports it. A scoped enum defaults to int.

enum class Opcode : std::uint8_t { Nop = 0, Load = 1, Store = 2, Halt = 255 };

enum class BigId : std::int64_t { First = 1 };

int main() {
    std::cout << "sizeof(Opcode) = " << sizeof(Opcode) << " byte\n";    // 1
    std::cout << "sizeof(BigId)  = " << sizeof(BigId)  << " bytes\n";   // 8

    // The underlying type is a real type you can query and cast to.
    using U = std::underlying_type<Opcode>::type;
    std::cout << "Opcode underlying is 1-byte unsigned? "
              << std::is_same<U, std::uint8_t>::value << "\n";          // 1

    Opcode op = Opcode::Halt;
    std::cout << "Halt = " << static_cast<int>(op) << "\n";             // 255

    // Pinning the type also lets a scoped enum be forward-declared elsewhere,
    // since its size is known without the full definition.
    return 0;
}
