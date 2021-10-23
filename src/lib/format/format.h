#pragma once

#include <cassert>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QVarLengthArray>

#include <format>

namespace detail {

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
struct formatter<QStringView, wchar_t> : public std::formatter<std::wstring_view, wchar_t> {
    using base_type = std::formatter<std::wstring_view, wchar_t>;

    template <class FormatContext>
    auto format(QStringView value, FormatContext& ctx) {
        detail::QStringToWcharConversionBuffer buffer(value);
        return base_type::format(buffer.view(), ctx);
    }
};

template<>
struct formatter<QString, wchar_t> : public formatter<QStringView, wchar_t> {};

} // namespace std

template<class... Args>
QString format(QStringView formatString, Args&&... args) {
    QString result;
    detail::QStringToWcharConversionBuffer buffer(formatString);
    detail::QStringBackInsertConvertingProxy resultProxy(&result);
    std::format_to(std::back_inserter(resultProxy), buffer.view(), std::forward<Args>(args)...);
    return result;
}
