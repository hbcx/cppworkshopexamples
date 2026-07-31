#include <iomanip>
#include <iostream>
#include <string>

// One process-wide settings store for the whole game. Typed accessors with
// defaults, so a caller never reads an unset value, and a private constructor
// that seeds the defaults exactly once (Meyers Singleton).
class GameSettings {
public:
    static GameSettings& instance() {
        static GameSettings inst;
        return inst;
    }

    GameSettings(const GameSettings&) = delete;
    GameSettings& operator=(const GameSettings&) = delete;

    float masterVolume() const { return masterVolume_; }
    void setMasterVolume(float v) { masterVolume_ = v; }

    const std::string& resolution() const { return resolution_; }
    void setResolution(std::string r) { resolution_ = std::move(r); }

    bool fullscreen() const { return fullscreen_; }
    void setFullscreen(bool f) { fullscreen_ = f; }

private:
    GameSettings() { std::cout << "[settings] defaults loaded\n"; }

    // Defaults baked in -- typed members, not a stringly-typed map, so a
    // misspelled setting is a compile error rather than a silent default.
    float masterVolume_ = 0.8f;
    std::string resolution_ = "1920x1080";
    bool fullscreen_ = false;
};

// Two unrelated subsystems. Neither is handed a settings object; both reach the
// same store through instance(), so they always agree on the current values.
void audioSubsystem() {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "[audio]  master volume = " << GameSettings::instance().masterVolume() << '\n';
}
void renderSubsystem() {
    const GameSettings& s = GameSettings::instance();
    std::cout << "[render] " << s.resolution()
              << " fullscreen=" << (s.fullscreen() ? "on" : "off") << '\n';
}

int main() {
    // Construct the store up front so its defaults load before any subsystem runs.
    GameSettings::instance();

    // The subsystems read the shared defaults.
    audioSubsystem();
    renderSubsystem();

    // The user changes settings in one place...
    std::cout << "-- user changes settings --\n";
    GameSettings::instance().setMasterVolume(0.5f);
    GameSettings::instance().setResolution("2560x1440");
    GameSettings::instance().setFullscreen(true);

    // ...and every subsystem observes the change, no wiring between them.
    audioSubsystem();
    renderSubsystem();
    return 0;
}
