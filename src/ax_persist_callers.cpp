#include "ax_persist.hpp"

#include <cstddef>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace amiberry::ax {

bool PersistAmigaSaveStateFile(
    std::string_view state_stem,
    std::span<const std::byte> state_bytes) noexcept {
    const fs::path state_file =
        fs::path("/userdata/saves/amiga") / (std::string(state_stem) + ".uss");
    return PersistAmigaStateAtomic(state_file, state_bytes);
}

bool PersistAmigaNvramFile(
    std::string_view nvram_name,
    std::span<const std::byte> nvram_bytes) noexcept {
    const fs::path nvram_file =
        fs::path("/userdata/saves/amiga") / nvram_name;
    return PersistAmigaStateAtomic(nvram_file, nvram_bytes);
}

bool PersistAmigaProfileConfig(
    std::string_view profile_name,
    std::string_view uae_config_text) noexcept {
    const fs::path config_file =
        fs::path("/userdata/system/configs/amiberry") / (std::string(profile_name) + ".uae");
    return PersistAmigaConfigAtomic(config_file, uae_config_text);
}

} // namespace amiberry::ax
