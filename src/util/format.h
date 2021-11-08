#pragma once

#include <cassert>

#include <format>
#include <string_view>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QVarLengthArray>
#include <QtCore/QDebug>

#include "exception.h"

/**
 * This is a pretty bad hack. We're basically forward-declaring a function from Qt internals. But whatever.
 */
struct QUtf8 {
    static char* convertFromUnicode(char* out, QStringView in, QStringConverter::State* state);
    static QChar* convertToUnicode(QChar* out, QByteArrayView in, QStringConverter::State* state);
};

namespace detail {

template<class T>
concept debug_streamable = requires (QDebug a, T b) { a << b; };

inline void appendUtf16ToUtf8(QStringView src, QByteArray* dst) {
    size_t oldSize = dst->size();
    dst->resize(dst->size() + 3 * src.size()); /* QByteArray::resize doesn't initialize new bytes! */
    QStringConverter::State state;
    char* end = QUtf8::convertFromUnicode(dst->data() + oldSize, src, &state);
    dst->resize(end - dst->data());
}

inline void appendUft8ToUtf16(QByteArrayView src, QString* dst) {
    size_t oldSize = dst->size();
    dst->resize(dst->size() + src.size()); /* QString::resize doesn't initialize new bytes! */
    QStringConverter::State state;
    QChar* end = QUtf8::convertToUnicode(dst->data() + oldSize, src, &state);
    dst->resize(end - dst->data());
}

class StringFormatState {
public:
    using value_type = wchar_t;

    StringFormatState() : m_debug(&m_result) {
        m_debug = m_debug.nospace();
    }

    void append(wchar_t character) {
        /* Surrogates are not supported. */
        assert(sizeof(wchar_t) == sizeof(char16_t) || !QChar::requiresSurrogates(static_cast<char32_t>(character)));

        m_result.push_back(QChar(static_cast<char16_t>(character)));
    }

    void append(QStringView string) {
        m_result.append(string);
    }

    void append(QByteArrayView utf8) {
        appendUft8ToUtf16(utf8, &m_result);
    }

    template<class T>
    void appendDebug(const T& value) {
        m_debug << value;
    }

    QString takeResult() {
        return std::move(m_result);
    }

private:
    QString m_result;
    QDebug m_debug;
};

class ByteArrayFormatState {
public:
    using value_type = wchar_t;

    ByteArrayFormatState() : m_debug(&m_debugBuffer) {
        m_debug = m_debug.nospace();
    }

    void append(char character) {
        m_result.push_back(character);
    }

    void append(QByteArrayView string) {
        m_result.append(string);
    }

    template<class T>
    void appendDebug(const T& value) {
        m_debugBuffer.resize(0); /* QString::clear deallocates, this is not what we need here. */
        m_debug << value;
        appendUtf16ToUtf8(m_debugBuffer, &m_result);
    }

    QByteArray takeResult() {
        return std::move(m_result);
    }

private:
    QByteArray m_result;
    QString m_debugBuffer;
    QDebug m_debug;
};

template<class State>
class FormatOutputIterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using pointer = void;
    using reference = void;
    using difference_type = ptrdiff_t;

    FormatOutputIterator() = default;

    explicit FormatOutputIterator(State* state) : m_state(state) {}

    FormatOutputIterator& operator=(typename State::value_type character) {
        m_state->append(character);
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

    template<class T>
    FormatOutputIterator append(const T& value) {
        m_state->append(value);
        return *this;
    }

    template<class T>
    FormatOutputIterator appendDebug(const T& value) {
        m_state->appendDebug(value);
        return *this;
    }

private:
    State* m_state = nullptr;
};

template<class T>
struct is_format_output_iterator : std::false_type {};

template<class State>
struct is_format_output_iterator<FormatOutputIterator<State>> : std::true_type {};

template<class FormatContext>
void requireFormatOutputIterator(FormatContext&) {
    static_assert(
        is_format_output_iterator<typename FormatContext::iterator>::value,
        "Formatting this type is supported only through ::sformat/::bformat calls."
    );
}

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
 * Formatter that forward to State::append.
 */
template<class T>
struct AppendFormatter : EmptySpecFormatter {
    template <class FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        requireFormatOutputIterator(ctx);
        return ctx.out().append(value);
    }
};

/**
 * Formatter that forwards to State::appendDebug.
 */
template<class T>
struct DebugFormatter : EmptySpecFormatter {
    template <class FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        requireFormatOutputIterator(ctx);
        return ctx.out().appendDebug(value);
    }
};

} // namespace detail

namespace std {

template<>
struct formatter<QStringView, wchar_t> : detail::AppendFormatter<QStringView> {};

template<>
struct formatter<QString, wchar_t> : detail::AppendFormatter<QString> {};

template<>
struct formatter<QByteArrayView, wchar_t> : detail::AppendFormatter<QByteArrayView> {};

template<>
struct formatter<QByteArray, wchar_t> : detail::AppendFormatter<QByteArray> {};

template<class T> requires
    detail::debug_streamable<T> &&
    std::is_class_v<std::remove_cvref_t<T>>
struct formatter<T, wchar_t> : detail::DebugFormatter<T> {};

template<>
struct formatter<QByteArrayView, char> : detail::AppendFormatter<QByteArrayView> {};

template<>
struct formatter<QByteArray, char> : detail::AppendFormatter<QByteArray> {};

template<class T> requires
    detail::debug_streamable<T> &&
    std::is_class_v<std::remove_cvref_t<T>> &&
    (!std::is_same_v<std::remove_cvref_t<T>, QString>) &&
    (!std::is_same_v<std::remove_cvref_t<T>, QStringView>)
struct formatter<T, char> : detail::DebugFormatter<T> {};

} // namespace std


/**
 * `std::format` alternative that formats into `QString`.
 */
template<class... Args>
QString sformat(std::wstring_view formatString, const Args&... args) {
    using namespace detail;

    StringFormatState state;
    std::vformat_to(
        FormatOutputIterator<StringFormatState>(&state),
        formatString,
        std::make_format_args<std::basic_format_context<FormatOutputIterator<StringFormatState>, wchar_t>>(args...)
    );
    return state.takeResult();
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

    ByteArrayFormatState state;
    std::vformat_to(
        FormatOutputIterator<ByteArrayFormatState>(&state),
        formatString,
        std::make_format_args<std::basic_format_context<FormatOutputIterator<ByteArrayFormatState>, char>>(args...)
    );
    return state.takeResult();
}

template<class... Args>
QByteArray bformat(QByteArrayView formatString, const Args&... args) {
    return bformat(std::string_view(formatString.data(), formatString.size()), args...);
}

template<class... Args>
QByteArray bformat(const char* formatString, const Args&... args) {
    return bformat(std::string_view(formatString), args...);
}

