#pragma once

#include <QtCore/QMetaObject>

#include <util/exception.h>

// TODO: we need better exception messages here. Provide method name in message!
// yeah, when we get to exception i18n.

class MetaCallException : public Exception {
    Q_DECLARE_TR_FUNCTIONS(MetaCallException)
public:
    MetaCallException(const QMetaObject* type, const QByteArray& method, const QString& message) :
        Exception(message),
        m_type(type),
        m_method(method)
    {}

    const QMetaObject* type() const {
        return m_type;
    }

    const QByteArray& method() const {
        return m_method;
    }

private:
    const QMetaObject* m_type;
    QByteArray m_method;
};
