#include "meta_closure.h"

#include "meta_object_extension.h"
#include "meta_call_exception.h"

MetaClosure::MetaClosure(QObject* target, const QByteArray& method, const QVariantList& args) {
    init(target, method, args);
}

void MetaClosure::init(QObject* target, const QByteArray& method, const QVariantList& args) {
    assert(target);
    m_target = nullptr;

    m_method = MetaObjectExtension::forObject(target)->methodByName(method);
    if (!m_method.isValid())
        xthrow MetaCallException(target->metaObject(), method, MetaCallException::tr("Method doesn't exist.")); // TODO: better message text!

    m_args = args;
    MetaMethodInvoker::prepareInvocation(m_method, &m_args, &m_ctx); /* Will throw on error. */

    m_target = target; /* All went OK. */
}

bool MetaClosure::isValid() const {
    return m_target;
}

QVariant MetaClosure::invoke() const {
    assert(isValid());

    return MetaMethodInvoker::invoke(m_target, m_method, &m_ctx);
}
