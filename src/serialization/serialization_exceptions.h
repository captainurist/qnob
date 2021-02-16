#pragma once

#include <typeinfo>

#include <QtCore/QStringView>

[[noreturn]] void throwSerializationException(const std::type_info& type);
[[noreturn]] void throwDeserializationException(QStringView invalidValue, const std::type_info& typeInfo);
