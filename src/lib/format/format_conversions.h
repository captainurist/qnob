#pragma once

#include <cassert>

#include <string_view>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QVarLengthArray>

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
