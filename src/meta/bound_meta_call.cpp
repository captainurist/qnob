#include "bound_meta_call.h"

#include <utility/exception.h>

#include "meta_object_extension.h"

void BoundMetaCall::bind(QObject* target, QByteArray method, const QVariantList& args) {
    m_target = nullptr;

    m_method = MetaObjectExtension::forObject(target)->methodByName(method);
    if (!m_method.isValid())
        qthrow Exception(QString()); // TODO

    m_args = args;
    MetaMethodInvoker::prepareInvocation(m_method, &m_args, &m_ctx);

    m_target = target; /* All went OK. */
}

QVariant BoundMetaCall::invoke() {
    assert(m_target);
    return MetaMethodInvoker::invoke(m_target, m_method, &m_ctx);
}
