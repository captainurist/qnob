#pragma once

#include <unordered_map>

#include <QtCore/QByteArray>
#include <QtCore/QHashFunctions>
#include <QtCore/QMetaMethod>

class MetaObjectExtension {
public:
    MetaObjectExtension(const QMetaObject* metaObject);
    ~MetaObjectExtension();

    static const MetaObjectExtension* forObject(QObject* object);

    /**
     * \param name                      Method name.
     * \returns                         A method for the given name. Returns invalid method if the method is overloaded
     *                                  or doesn't exist.
     */
    QMetaMethod methodByName(const QByteArray& name) const;

private:
    std::unordered_map<QByteArray, QMetaMethod> m_methodByName;
};

