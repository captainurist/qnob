#pragma once

#include <cassert>

#include <format>
#include <type_traits>
#include <string_view>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QVarLengthArray>
#include <QtCore/QDebug>

#include <util/explicit.h>

namespace detail {

template<class T>
concept debug_streamable = requires (QDebug a, T b) { a << b; };

class StringFormatOps {
public:
    template<class OutputIterator>
    static OutputIterator copy(Explicit<const QString&> string, OutputIterator out) {
        return copy(QStringView(*string), out);
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QStringView> string, OutputIterator out) {
        return std::transform(string->begin(), string->end(), out, [](QChar c) { return static_cast<wchar_t>(c.unicode()); });
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QUtf8StringView> string, OutputIterator out) {
        return copy(string->toString(), out);
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QLatin1String> string, OutputIterator out) {
        return std::copy(string->begin(), string->end(), out);
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QAnyStringView> string, OutputIterator out) {
        return string->visit([&](auto view) {
            return copy(view, out);
        });
    }
};

class ByteArrayFormatOps {
public:
    template<class OutputIterator>
    static OutputIterator copy(Explicit<const QByteArray&> string, OutputIterator out) {
        return copy(QByteArrayView(*string), out);
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QByteArrayView> string, OutputIterator out) {
        return std::copy(string->begin(), string->end(), out);
    }

    template<class OutputIterator>
    static OutputIterator copy(Explicit<QUtf8StringView> string, OutputIterator out) {
        return copy(QByteArrayView(string->data(), string->size()), out);
    }

    template<class OutputIterator>
    static OutputIterator append(Explicit<QLatin1String> string, OutputIterator out) {
        return copy(QByteArrayView(string->data(), string->size()), out);
    }
};

class StringBuffer {
public:
    void push_back(Explicit<wchar_t> character) {
        /* Surrogates are not supported. */
        assert(sizeof(wchar_t) == sizeof(char16_t) || !QChar::requiresSurrogates(static_cast<char32_t>(*character)));

        m_value.push_back(QChar(static_cast<char16_t>(*character)));
    }

    void push_back(Explicit<char> character) {
        /* This function is needed for STL formatters (int, float, etc.) to work. */
        assert(*character >= 0 && *character <= 0x7f); /* Sane Latin-1 chars please. */

        m_value.push_back(QLatin1Char(*character));
    }

    QString take() {
        return std::move(m_value);
    }

private:
    QString m_value;
};

class ByteArrayBuffer {
public:
    void push_back(Explicit<char> character) {
        m_value.push_back(*character);
    }

    QByteArray take() {
        return std::move(m_value);
    }

private:
    QByteArray m_value;
};

template<class Buffer>
class FormatOutputIterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using pointer = void;
    using reference = void;
    using difference_type = ptrdiff_t;

    FormatOutputIterator() = default;
    FormatOutputIterator& operator=(const FormatOutputIterator& other) = default;

    explicit FormatOutputIterator(Buffer *buffer) : m_buffer(buffer) {}

    template<class Character>
    FormatOutputIterator& operator=(Character character) {
        /* It's important that we pass the character type down to the buffer object so that we could check that
         * it's actually supported. */
        m_buffer->push_back(character);
        return *this;
    }

    [[nodiscard]] FormatOutputIterator& operator*() {
        return *this;
    }

    FormatOutputIterator& operator++() {
        return *this;
    }

    FormatOutputIterator operator++(int) {
        return *this;
    }

private:
    Buffer* m_buffer = nullptr;
};

/**
 * Helper class that provides a `std::wstring_view` over a `QString`, performing conversion if necessary.
 */
struct QStringToWcharConversionBuffer {
public:
    QStringToWcharConversionBuffer(QStringView source) {
        if (sizeof(wchar_t) == sizeof(QChar)) {
            const wchar_t* data = reinterpret_cast<const wchar_t*>(source.data());
            m_view = std::wstring_view(data, data + source.size());
        } else {
            m_buffer.reserve(source.size());
            qsizetype size = source.toWCharArray(m_buffer.data());
            m_view = std::wstring_view(m_buffer.data(), size);
        }
    }

    std::wstring_view view() const {
        return m_view;
    }

private:
    QVarLengthArray<wchar_t, 256> m_buffer;
    std::wstring_view m_view;
};

/**
 * Formatter that expects an empty format spec.
 */
struct EmptySpecFormatter {
    template<class ParseContext>
    typename ParseContext::iterator parse(ParseContext& ctx) {
        auto pos = ctx.begin();
        if (pos == ctx.end())
            return pos;
        if (*pos != '}')
            throw std::format_error("Only empty format specs are supported.");
        return pos;
    }
};

/**
 * Formatter that forward to Ops::copy.
 */
template<class T>
struct AppendFormatter : EmptySpecFormatter {
    template <class FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        if constexpr (std::is_same_v<typename FormatContext::char_type, char>) {
            return ByteArrayFormatOps::copy(value, ctx.out());
        } else {
            return StringFormatOps::copy(value, ctx.out());
        }
    }
};

/**
 * Formatter that uses QDebug.
 */
template<class T>
struct DebugFormatter : EmptySpecFormatter {
    template <class FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        QString string;
        QDebug debug(&string);
        debug.nospace();
        debug << value;

        if constexpr (std::is_same_v<typename FormatContext::char_type, char>) {
            return ByteArrayFormatOps::copy(string.toUtf8(), ctx.out());
        } else {
            return StringFormatOps::copy(string, ctx.out());
        }
    }
};

struct UnknownEncodingFailureFormatter : EmptySpecFormatter {
    template <class T, class FormatContext>
    auto format(const T&, FormatContext&) {
        static_assert(std::is_same_v<T, T>, "Formatting bytes into QString is not supported, use QLatin1String / QUtf8StringView to specify encoding explicitly.");
    }
};

} // namespace detail


template<>
struct std::formatter<QStringView, wchar_t> : detail::AppendFormatter<QStringView> {};

template<>
struct std::formatter<QString, wchar_t> : detail::AppendFormatter<QString> {};

template<>
struct std::formatter<QLatin1String, wchar_t> : detail::AppendFormatter<QLatin1String> {};

template<>
struct std::formatter<QUtf8StringView, wchar_t> : detail::AppendFormatter<QUtf8StringView> {};

template<>
struct std::formatter<QAnyStringView, wchar_t> : detail::AppendFormatter<QAnyStringView> {};

template<>
struct std::formatter<QByteArrayView, wchar_t> : detail::UnknownEncodingFailureFormatter {};

template<>
struct std::formatter<QByteArray, wchar_t> : detail::UnknownEncodingFailureFormatter {};

template<class T> requires
    detail::debug_streamable<T> &&
    std::is_class_v<std::remove_cvref_t<T>> &&
    (!std::is_same_v<std::remove_cvref_t<T>, std::wstring>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, std::wstring_view>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QByteArray>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QByteArrayView>)
struct std::formatter<T, wchar_t> : detail::DebugFormatter<T> {};

template<>
struct std::formatter<QByteArrayView, char> : detail::AppendFormatter<QByteArrayView> {};

template<>
struct std::formatter<QByteArray, char> : detail::AppendFormatter<QByteArray> {};

template<>
struct std::formatter<QLatin1String, char> : detail::AppendFormatter<QByteArray> {};

template<>
struct std::formatter<QUtf8StringView, char> : detail::AppendFormatter<QUtf8StringView> {};

template<class T> requires
    detail::debug_streamable<T> &&
    std::is_class_v<std::remove_cvref_t<T>> &&
    (!std::is_same_v<std::remove_cvref_t<T>, std::string>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, std::string_view>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QString>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QStringView>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QAnyStringView>)
struct std::formatter<T, char> : detail::DebugFormatter<T> {};


/**
 * `std::format` alternative that formats into `QString`.
 */
template<class... Args>
QString sformat(std::wstring_view formatString, const Args&... args) {
    using namespace detail;

    StringBuffer buffer;
    std::vformat_to(
        FormatOutputIterator<StringBuffer>(&buffer),
        formatString,
        std::make_wformat_args(args...)
    );
    return buffer.take();
}

template<class... Args>
QString sformat(QStringView formatString, const Args&... args) {
    return sformat(detail::QStringToWcharConversionBuffer(formatString).view(), args...);
}

template<class... Args>
QString sformat(const wchar_t* formatString, const Args&... args) {
    return sformat(std::wstring_view(formatString), args...);
}


/**
 * `std::format` alternative that formats into `QByteArray`.
 */
template<class... Args>
QByteArray bformat(std::string_view formatString, const Args&... args) {
    using namespace detail;

    ByteArrayBuffer buffer;
    std::vformat_to(
        FormatOutputIterator<ByteArrayBuffer>(&buffer),
        formatString,
        std::make_format_args(args...)
    );
    return buffer.take();
}

template<class... Args>
QByteArray bformat(QByteArrayView formatString, const Args&... args) {
    return bformat(std::string_view(formatString.data(), formatString.size()), args...);
}

template<class... Args>
QByteArray bformat(const char* formatString, const Args&... args) {
    return bformat(std::string_view(formatString), args...);
}

