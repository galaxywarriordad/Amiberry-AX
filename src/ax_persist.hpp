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
