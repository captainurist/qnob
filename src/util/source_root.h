#pragma once

#include <string_view>

namespace detail {
    constexpr const char absoluteCurrentFilePath[] = __FILE__;
    constexpr const char relativeCurrentFilePath[] = "src/util/source_root.h";

    constexpr const char* rootRelativeSourcePathInternal(const char* sourcePath) {
        return sourcePath + sizeof(absoluteCurrentFilePath) - sizeof(relativeCurrentFilePath);
    }
}

/**
 * \returns                             Source root, as used when this file was compiled. Includes trailing slash.
 */
consteval std::string_view sourceRoot() {
    return { detail::absoluteCurrentFilePath, sizeof(detail::absoluteCurrentFilePath) - sizeof(detail::relativeCurrentFilePath) };
}

/**
 * \param sourcePath                    Full path to the source file.
 * \returns                             Path to the provided source file relative to the source root. Returns the path
 *                                      unchanged if the provided file is not in the source tree (e.g. it's a file from
 *                                      Qt sources).
 */
inline std::string_view rootRelativeSourcePath(std::string_view sourcePath) {
    return sourcePath.starts_with(sourceRoot()) ? detail::rootRelativeSourcePathInternal(sourcePath.data()) : sourcePath;
}
