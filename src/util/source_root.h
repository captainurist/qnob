#pragma once

#include <string_view>

namespace detail {
    constexpr const char absoluteCurrentFilePath[] = __FILE__;
    constexpr const char relativeCurrentFilePath[] = "src/util/source_root.h";
}

/**
 * \returns                             Source root.
 */
constexpr std::string_view sourceRoot() {
    return { detail::absoluteCurrentFilePath, sizeof(detail::absoluteCurrentFilePath) - sizeof(detail::relativeCurrentFilePath) };
}

/**
 * \param fileName                      Filename as returned by `__FILE__` macro from inside the source root.
 * \returns                             Source root-relative file path.
 */
constexpr const char* sourceRootRelativeFilePath(const char* fileName) {
    return fileName + sizeof(detail::absoluteCurrentFilePath) - sizeof(detail::relativeCurrentFilePath);
}
