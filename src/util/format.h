#pragma once

#include <cassert>

#include <format>
#include <string_view>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QVarLengthArray>
#include <QtCore/QDebug>


template<class T>
concept debug_streamable = requires (QDebug a, T b) { a << b; };

namespace detail {

/**
 * Helper class that can be used with `std::back_inserter` to insert `wchar_t` chars directly into `QString`.
 */
struct QStringBackInsertConvertingProxy {
public:
    using value_type = wchar_t;

    QStringBackInsertConvertingProxy(QString* target) : m_target(target) {}

    void push_back(wchar_t character) {
        /* Surrogates are not supported. */
        assert(sizeof(wchar_t) == sizeof(char16_t) || !QChar::requiresSurrogates(static_cast<char32_t>(character)));

        m_target->push_back(QChar(static_cast<char16_t>(character)));
    }

private:
    QString* m_target;
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

} // namespace detail


namespace std {

template<>
struct formatter<QStringView, wchar_t> : public formatter<std::wstring_view, wchar_t> {
    using base_type = formatter<std::wstring_view, wchar_t>;

    template <class FormatContext>
    auto format(QStringView value, FormatContext& ctx) {
        detail::QStringToWcharConversionBuffer buffer(value);
        return base_type::format(buffer.view(), ctx);
    }
};

template<>
struct formatter<QString, wchar_t> : public formatter<QStringView, wchar_t> {};

template<>
struct formatter<QByteArrayView, wchar_t> : public formatter<QStringView, wchar_t> {
    using base_type = formatter<QStringView, wchar_t>;

    template <typename FormatContext>
    auto format(QByteArrayView value, FormatContext& ctx) {
        return base_type::format(QString::fromUtf8(value), ctx);
    }
};

template<>
struct formatter<QByteArray, wchar_t> : public formatter<QByteArrayView, wchar_t> {};

template<class T> requires debug_streamable<T> && std::is_class_v<std::remove_cvref_t<T>>
struct formatter<T, wchar_t> : public formatter<QStringView, wchar_t> {
    using base_type = formatter<QStringView, wchar_t>;

    template <typename FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        QString buffer;
        QDebug(&buffer).nospace() << value;
        return base_type::format(buffer, ctx);
    }
};

template<>
struct formatter<QByteArrayView, char> : public formatter<std::string_view, char> {
    using base_type = formatter<std::string_view, char>;

    template <typename FormatContext>
    auto format(QByteArrayView value, FormatContext& ctx) {
        return base_type::format(std::string_view(value.data(), value.size()), ctx);
    }
};

template<>
struct formatter<QByteArray, char> : public formatter<QByteArrayView, char> {};

template<class T> requires debug_streamable<T>&& std::is_class_v<std::remove_cvref_t<T>>
struct formatter<T, char> : public formatter<QByteArrayView, char> {
    using base_type = formatter<QByteArrayView, char>;

    template <typename FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        QString buffer;
        QDebug(&buffer).nospace() << value;
        return base_type::format(buffer.toUtf8(), ctx);
    }
};

} // namespace std


/**
 * `std::format` alternative that formats into `QString`.
 */
template<class... Args>
QString sformat(std::wstring_view formatString, Args&&... args) {
    QString result;
    detail::QStringBackInsertConvertingProxy resultProxy(&result);
    std::format_to(std::back_inserter(resultProxy), formatString, std::forward<Args>(args)...);
    return result;
}

template<class... Args>
QString sformat(QStringView formatString, Args&&... args) {
    return sformat(detail::QStringToWcharConversionBuffer(formatString).view(), std::forward<Args>(args)...);
}

template<class... Args>
QString sformat(const wchar_t* formatString, Args&&... args) {
    return sformat(std::wstring_view(formatString), std::forward<Args>(args)...);
}

/**
 * `std::format` alternative that formats into a `QByteArray`.
 */
template<class... Args>
QByteArray bformat(std::string_view formatString, Args&&... args) {
    QByteArray result;
    std::format_to(std::back_inserter(result), formatString, std::forward<Args>(args)...);
    return result;
}

template<class... Args>
QByteArray bformat(QByteArrayView formatString, Args&&... args) {
    return bformat(std::string_view(formatString.data(), formatString.size()), std::forward<Args>(args)...);
}

template<class... Args>
QByteArray bformat(const char* formatString, Args&&... args) {
    return bformat(std::string_view(formatString), std::forward<Args>(args)...);
}

