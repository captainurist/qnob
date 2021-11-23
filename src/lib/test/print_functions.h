#pragma once

#include <ostream>

#include <QtCore/QString>

inline void PrintTo(const QString& string, std::ostream* stream) {
    *stream << "\"" << qUtf8Printable(string) << "\"";
}
