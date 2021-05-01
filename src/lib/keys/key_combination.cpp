#include "key_combination.h"

#include <unordered_map>

#include <QtCore/QString>

#include <util/map_access.h>
#include <util/bad_cast_exception.h>

#include "key_table.h"

class KeyNameMapper {
public:
    KeyNameMapper() {
        for (const auto& [key, name] : KeyMapping) {
            if (!m_nameByKey.contains(key))
                m_nameByKey[key] = QLatin1String(name);

            m_keyByName[QString::fromLatin1(name).toLower()] = key;
        }
    }

    QString name(QKeyCombination key) const {
        return value_or(m_nameByKey, key, QString());
    }

    QKeyCombination key(QStringView name) const {
        return value_or(m_keyByName, name.toString().toLower(), Qt::Key_unknown);
    }

private:
    std::unordered_map<QKeyCombination, QString> m_nameByKey;
    std::unordered_map<QString, QKeyCombination> m_keyByName;
};

Q_GLOBAL_STATIC(KeyNameMapper, g_keyNameMapper)

void serialize(const QKeyCombination& value, QString* target) {
    QKeyCombination nakedKey(value.keyboardModifiers() & Qt::KeypadModifier, value.key());
    QString result = g_keyNameMapper->name(nakedKey);
    if (result.isEmpty())
        qthrow BadCastException(BadCastException::tr("Couldn't serialize key combination '0x%1'").arg(value.toCombined(), 8, 16, QLatin1Char('0')));



    *target = result;
}

void deserialize(QStringView string, QKeyCombination* target) {
    QKeyCombination result = g_keyNameMapper->key(string);
    if (result == Qt::Key_unknown)
        qthrow BadCastException(BadCastException::tr("Invalid key combination '%1'").arg(string));
}
