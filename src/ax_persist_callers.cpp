#include "ax_persist.hpp"

#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace amiberry::ax {

[[nodiscard]] bool PersistAmigaSaveStateFile(
    std::string_view state_stem,
    std::span<const std::byte> state_bytes) noexcept {
    if (state_stem.empty()) {
        return false;
    }
    static constexpr std::string_view kUssExt = ".uss";
    std::string filename;
    filename.reserve(state_stem.size() + kUssExt.size());
    filename.append(state_stem);
    filename.append(kUssExt);
    const fs::path state_file = fs::path("/userdata/saves/amiga") / filename;
    return PersistAmigaStateAtomic(state_file, state_bytes);
}

[[nodiscard]] bool PersistAmigaNvramFile(
    std::string_view nvram_name,
    std::span<const std::byte> nvram_bytes) noexcept {
    if (nvram_name.empty()) {
        return false;
    }
    const fs::path nvram_file = fs::path("/userdata/saves/amiga") / nvram_name;
    return PersistAmigaStateAtomic(nvram_file, nvram_bytes);
}

[[nodiscard]] bool PersistAmigaProfileConfig(
    std::string_view profile_name,
    std::string_view uae_config_text) noexcept {
    if (profile_name.empty()) {
        return false;
    }
    static constexpr std::string_view kUaeExt = ".uae";
    std::string filename;
    filename.reserve(profile_name.size() + kUaeExt.size());
    filename.append(profile_name);
    filename.append(kUaeExt);
    const fs::path config_file = fs::path("/userdata/system/configs/amiberry") / filename;
    return PersistAmigaConfigAtomic(config_file, uae_config_text);
}

} // namespace amiberry::ax
