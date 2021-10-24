#pragma once

#include <format>
#include <type_traits>

#include <QtCore/QString>
#include <QtCore/QStringView>

#include "format_conversions.h"

class QDebug;

template<class T>
concept debug_streamable = requires (QDebug a, T b) { a << b; };


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
    using base = formatter<QStringView, wchar_t>;

    template <typename FormatContext>
    auto format(QByteArrayView value, FormatContext& ctx) {
        return base::format(QString::fromUtf8(value), ctx);
    }
};

template<>
struct formatter<QByteArray, wchar_t> : public formatter<QByteArrayView, wchar_t> {};


template<class T> requires debug_streamable<T> && std::is_class_v<T>
class formatter<T, wchar_t> : public formatter<QStringView, wchar_t> {
    using base = formatter<QStringView, wchar_t>;

    template <typename FormatContext>
    auto format(const T& value, FormatContext& ctx) {
        QString buffer;
        QDebug(&buffer) << value;
        return base_type::format(buffer, ctx);
    }
};


} // namespace std


