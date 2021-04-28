#include "bound_meta_call.h"

#include "meta_object_extension.h"
#include "meta_call_exception.h"

void BoundMetaCall::bind(QObject* target, QByteArray method, const QVariantList& args) {
    m_target = nullptr;

    m_method = MetaObjectExtension::forObject(target)->methodByName(method);
    if (!m_method.isValid())
        qthrow MetaCallException(target->metaObject(), method, MetaCallException::tr("Method doesn't exist.")); // TODO: better message text!

    m_args = args;
    MetaMethodInvoker::prepareInvocation(m_method, &m_args, &m_ctx);

    m_target = target; /* All went OK. */
}

QVariant BoundMetaCall::invoke() {
    assert(m_target);

    return MetaMethodInvoker::invoke(m_target, m_method, &m_ctx);
}
