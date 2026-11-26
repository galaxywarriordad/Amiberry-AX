#pragma once

#include "ax/persist.hpp"

#include <cstddef>
#include <filesystem>
#include <span>
#include <string_view>

namespace amiberry::ax {

[[nodiscard]] inline bool PersistAmigaStateAtomic(
    const std::filesystem::path& state_file,
    std::span<const std::byte> serialized_state) noexcept {
    if (state_file.empty()) {
        return false;
    }

    const std::filesystem::path parent = state_file.parent_path();
    std::error_code ec;
    if (!parent.empty() && std::filesystem::exists(parent, ec) && !ec) {
        (void)ax::core::CleanupStaleTempFiles(parent, ".ax_tmp_");
    }

    ax::core::AtomicWriteOptions opts{};
    opts.file_mode = 0644;
    opts.create_parent_dirs = true;
    opts.fsync_file = true;
    opts.fsync_parent_dir = true;

    return ax::core::WriteFileAtomic(state_file, serialized_state, opts).ok;
}

[[nodiscard]] inline bool PersistAmigaConfigAtomic(
    const std::filesystem::path& config_file,
    std::string_view uae_config_text) noexcept {
    if (config_file.empty()) {
        return false;
    }

    const std::filesystem::path parent = config_file.parent_path();
    std::error_code ec;
    if (!parent.empty() && std::filesystem::exists(parent, ec) && !ec) {
        (void)ax::core::CleanupStaleTempFiles(parent, ".ax_tmp_");
    }

    ax::core::AtomicWriteOptions opts{};
    opts.file_mode = 0644;
    opts.create_parent_dirs = true;
    opts.fsync_file = true;
    opts.fsync_parent_dir = true;

    return ax::core::WriteFileAtomic(config_file, uae_config_text, opts).ok;
}

} // namespace amiberry::ax
```[cite: 24, 26]

---

### Updated `src/ax_persist_callers.cpp`

```cpp
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
```[cite: 25, 27]

With `Amiberry-AX` (`CMakeLists.txt`, non-Linux/non-Apple pruning, `ax_persist.hpp`, and `ax_persist_callers.cpp`) covered in one pass, are you ready to move on to **Step 4 (`Piccolo-AX` OS, boot, cache & release pipeline)**?
