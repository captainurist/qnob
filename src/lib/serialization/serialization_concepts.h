#pragma once

class QString;
class QStringView;

template<class T>
concept serializable = requires (T a, QString* b) { serialize(a, b); };

template<class T>
concept deserializable = requires (QStringView a, T* b) { deserialize(a, b); };
