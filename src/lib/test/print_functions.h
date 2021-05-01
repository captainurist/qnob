#pragma once

#include <ostream>

#include <QtCore/QString>

QT_BEGIN_NAMESPACE

inline void PrintTo(const QString& string, std::ostream* stream) {
    *stream << "\"" << qUtf8Printable(string) << "\"";
}

QT_END_NAMESPACE
