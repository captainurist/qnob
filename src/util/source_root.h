#pragma once

#include <string_view>

namespace detail {
    constexpr const char absoluteCurrentFilePath[] = __FILE__;
    constexpr const char relativeCurrentFilePath[] = "src/util/source_root.h";
}

/**
 * \returns                             Source root.
 */
consteval std::string_view sourceRoot() {
    return { detail::absoluteCurrentFilePath, sizeof(detail::absoluteCurrentFilePath) - sizeof(detail::relativeCurrentFilePath) };
}

/**
 * \param sourcePath                    Filename as returned by `__FILE__` macro from inside the source root.
 * \returns                             Source root-relative file path.
 */
constexpr const char* rootRelativeSourcePath(const char* sourcePath) {
    return sourcePath + sizeof(detail::absoluteCurrentFilePath) - sizeof(detail::relativeCurrentFilePath);
}

// TODO: docs
inline std::string_view relativeSourcePath(std::string_view sourcePath) {
    return sourcePath.starts_with(sourceRoot()) ? rootRelativeSourcePath(sourcePath.data()) : sourcePath;
}
