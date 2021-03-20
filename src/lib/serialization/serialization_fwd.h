#pragma once

class QString;
class QStringView;

#define QB_DECLARE_SERIALIZATION_FUNCTIONS(TYPE)                                                                        \
void serialize(const TYPE& value, QString* target);                                                                     \
void deserialize(QStringView string, TYPE* target);
