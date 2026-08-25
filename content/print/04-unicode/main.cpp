#include <print>

int main() {
    // std::print is Unicode-aware: it treats the format string and arguments as
    // UTF-8 and, on a Windows console, transcodes to what the console needs --
    // so accented and non-Latin text shows correctly, where std::cout would
    // often print mojibake unless the console code page was set up by hand.
    std::println("Polish:   {}", "Zażółć gęślą jaźń");
    std::println("German:   {}", "Grüße aus München");
    std::println("Japanese: {}", "こんにちは世界");
    std::println("Emoji:    {}", "❤");

    // When output is redirected to a file or pipe (not a console), print writes
    // the UTF-8 bytes straight through -- exactly what you want for a UTF-8 file.
    return 0;
}
