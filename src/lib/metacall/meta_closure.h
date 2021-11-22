#pragma once

#include <QtCore/QMetaMethod>
#include <QtCore/QVariant>

#include "meta_method_invoker.h"

/**
 * Closure around a call to a Qt meta method.
 *
 * Does type conversions & all the necessary checks. Throws `MetaCallException` in case of problems.
 */
class MetaClosure {
public:
    MetaClosure() = default;
    MetaClosure(const MetaClosure& other) = delete;
    MetaClosure(MetaClosure&& other) = default; /* Default move ctor will keep pointers stored in `m_ctx` valid. */

    /**
     * Initializes this closure.
     *
     * \param target                    Target object. Must not be null.
     * \param method                    Method name. If the method is overloaded or doesn't exist, `MetaCallException`
     *                                  will be thrown.
     * \param args                      Arguments to call the method with. If the arguments couldn't be converted to the
     *                                  types that the method expects, `MetaCallException` will be thrown.
     */
    MetaClosure(QObject* target, const QByteArray& method, const QVariantList& args);

    /**
     * \returns                         Whether this closure is initialized, and `invoke` can be safely called.
     */
    bool isValid() const;

    /**
     * Invokes this closure. Must be valid.
     *
     * \returns                         Whatever the method that was called has returned, or an invalid `QVariant` if it
     *                                  returns `void`.
     */
    QVariant invoke() const;

private:
    void init(QObject* target, const QByteArray& method, const QVariantList& args);

private:
    QObject* m_target = nullptr;
    QMetaMethod m_method;
    QVariantList m_args;
    MetaMethodInvoker::CallContext m_ctx;
};
