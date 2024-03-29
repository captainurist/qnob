#include "meta_object_extension.h"

#include <unordered_set>
#include <memory>

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QMutex>

#include <util/map_access.h>

class MetaObjectExtensionStorage {
public:
    const MetaObjectExtension* extension(const QMetaObject* metaObject) {
        QMutexLocker locker(&m_mutex);

        if (const MetaObjectExtension* result = value_ptr(m_extensionByMetaObject, metaObject))
            return result;

        return &m_extensionByMetaObject.emplace(metaObject, metaObject).first->second;
    }

private:
    std::unordered_map<const QMetaObject*, MetaObjectExtension> m_extensionByMetaObject;
    QMutex m_mutex;
};

Q_GLOBAL_STATIC(MetaObjectExtensionStorage, g_extensionStorage)

MetaObjectExtension::~MetaObjectExtension() {}

MetaObjectExtension::MetaObjectExtension(const QMetaObject* metaObject) {
    std::unordered_set<QByteArray> names;

    for (int i = 0; i < metaObject->methodCount(); i++) {
        QMetaMethod method = metaObject->method(i);
        QByteArray name = method.name();

        if (name.startsWith("_q_"))
            continue; /* Don't bother exposing reserved names. */

        if (names.contains(name)) {
            m_methodByName.erase(name);
        } else {
            m_methodByName[name] = method;
        }

        names.insert(name);
    }
}

const MetaObjectExtension* MetaObjectExtension::forObject(QObject* object) {
    constexpr const char* extensionPropertyName = "__meta_object_extension";

    QVariant result = object->property(extensionPropertyName);
    if (result.typeId() == QMetaType::VoidStar)
        return static_cast<const MetaObjectExtension*>(result.value<void*>());

    const QMetaObject* metaObject = object->metaObject();
    const MetaObjectExtension* extension = g_extensionStorage->extension(metaObject);

    object->setProperty(extensionPropertyName, QVariant::fromValue<void*>(const_cast<MetaObjectExtension*>(extension)));

    return extension;
}

QMetaMethod MetaObjectExtension::methodByName(const QByteArray& name) const {
    return value_or(m_methodByName, name);
}
