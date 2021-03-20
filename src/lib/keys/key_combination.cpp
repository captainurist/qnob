#include "key_combination.h"

#include <QtGui/QKeySequence>

#include <lib/serialization/enum_serialization.h>

QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(Qt::Key, ({
    {Qt::Key_Mouse1, "mouse1"}
}))

class KeyNameMap {
public:

private:
    void initializeDefaults() {
        Qt::Key
    }

private:
    std::unordered_map<Qt::Key, QString> m_nameByKey;
    std::unordered_map<QString, Qt::Key> m_keyByName;
};

static bool isMouseKey(Qt::Key key) {
	switch (key) {
    case Qt::Key_Mouse1:
    case Qt::Key_Mouse2:
    case Qt::Key_Mouse3:
    case Qt::Key_WheelUp:
    case Qt::Key_WheelDown:
        return true;
    default:
        return false;
	}
}

void serialize(const QKeyCombination& value, QString* target) {


}

void deserialize(QStringView string, QKeyCombination* target) {

}
