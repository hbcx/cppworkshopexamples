#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

// Anti-pattern: a variant for a set of types meant to GROW. Every new type must
// be added to the variant AND to every visit over it, across the codebase -- and
// outside code cannot extend it, because the alternative list is in your header.
struct Png {};
struct Jpeg {};
using Image = std::variant<Png, Jpeg>;   // add Gif? edit this + every visitor

std::string nameVariant(const Image& img) {
    return std::visit([](const auto& x) -> std::string {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, Png>) return "png";
        else                                  return "jpeg";
    }, img);   // a new alternative forces a new branch here (and everywhere else)
}

// Fix: an open set belongs behind a virtual interface. A new format is one new
// class -- no central list to edit, and other modules can add their own.
struct Format {
    virtual std::string name() const = 0;
    virtual ~Format() = default;
};
struct PngFmt : Format { std::string name() const override { return "png"; } };
struct GifFmt : Format { std::string name() const override { return "gif"; } };   // added freely

int main() {
    Image img = Jpeg{};
    std::cout << "variant:   " << nameVariant(img) << '\n';

    std::vector<std::unique_ptr<Format>> formats;
    formats.push_back(std::make_unique<PngFmt>());
    formats.push_back(std::make_unique<GifFmt>());
    for (const auto& f : formats) {
        std::cout << "interface: " << f->name() << '\n';
    }
    return 0;
}
