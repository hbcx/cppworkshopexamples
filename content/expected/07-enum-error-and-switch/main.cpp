#include <expected>
#include <iostream>
#include <map>
#include <string>

// The reasons reading a file can fail -- a fixed, closed set, so an enum class
// is the right error type: the caller handles each case explicitly.
enum class FileError {
    EmptyPath,
    NotFound,
    Empty,
    PermissionDenied,
};

// A stand-in filesystem so the example is deterministic (no real disk): a name
// maps to its contents; an absent name is "not found"; empty contents mean an
// empty file; "secret.txt" is treated as unreadable.
std::expected<std::string, FileError> readFile(const std::string& name) {
    static const std::map<std::string, std::string> fs = {
        {"config.ini", "port=8080"},
        {"empty.log", ""},
        {"secret.txt", "top secret"},
    };

    if (name.empty()) return std::unexpected(FileError::EmptyPath);
    if (name == "secret.txt") return std::unexpected(FileError::PermissionDenied);
    auto it = fs.find(name);
    if (it == fs.end()) return std::unexpected(FileError::NotFound);
    if (it->second.empty()) return std::unexpected(FileError::Empty);
    return it->second;
}

// The caller dispatches once on the result: value -> use it, error -> switch on
// the enum and react per case (here, log a line). A switch with no default lets
// the compiler warn if a new FileError is added and left unhandled.
void report(const std::string& name, const std::expected<std::string, FileError>& r) {
    if (r) {
        std::cout << "[ok]    " << name << ": read " << r->size()
                  << " bytes -> " << *r << '\n';
        return;
    }
    switch (r.error()) {
        case FileError::EmptyPath:
            std::cout << "[error] (no name given): path is empty\n";
            break;
        case FileError::NotFound:
            std::cout << "[error] " << name << ": file not found\n";
            break;
        case FileError::Empty:
            std::cout << "[warn]  " << name << ": file is empty\n";
            break;
        case FileError::PermissionDenied:
            std::cout << "[error] " << name << ": permission denied\n";
            break;
    }
}

int main() {
    for (const std::string& name : {std::string("config.ini"),
                                    std::string("missing.dat"),
                                    std::string("empty.log"),
                                    std::string("secret.txt"),
                                    std::string("")}) {
        report(name, readFile(name));
    }
    return 0;
}
