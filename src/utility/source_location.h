#pragma once

#ifndef _MSC_VER
#include <source_location>
#define __LOCATION__ std::source_location::current()
#else // _MSC_VER

#include <cstdint>

namespace std {

struct source_location {
public:
    /* current() unsupported. */

    constexpr source_location() {}

    constexpr source_location(const char* fileName, const char* functionName, const uint_least32_t lineNumber, const uint_least32_t columnOffset) noexcept:
        m_fileName(fileName),
        m_functionName(functionName),
        m_lineNumber(lineNumber),
        m_columnOffset(columnOffset)
    {
    }

    constexpr const char* file_name() const noexcept {
        return m_fileName;
    }

    constexpr const char* function_name() const noexcept {
        return m_functionName;
    }

    constexpr uint_least32_t line() const noexcept {
        return m_lineNumber;
    }

    constexpr std::uint_least32_t column() const noexcept {
        return m_columnOffset;
    }

private:
    const char* m_fileName = "";
    const char* m_functionName = "";
    std::uint_least32_t m_lineNumber = 0;
    std::uint_least32_t m_columnOffset = 0;
};

}

#define __LOCATION__ std::source_location(__FILE__, __FUNCTION__, __LINE__, 0)

#endif // _MSC_VER
