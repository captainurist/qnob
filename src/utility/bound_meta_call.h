#pragma once

#include <QtCore/QMetaMethod>
#include <QtCore/QVariant>

#include "meta_method_invoker.h"

class BoundMetaCall {
public:
    BoundMetaCall() {}

    void bind(QObject* m_target, QByteArray method, const QVariantList& args);

    QVariant invoke();

private:
    QObject* m_target;
    QMetaMethod m_method;
    QVariantList m_args;
    MetaMethodInvoker::CallContext m_ctx;
};
