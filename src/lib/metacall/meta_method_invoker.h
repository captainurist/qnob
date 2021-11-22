#pragma once

#include <array>

#include <QtCore/QVariant>
#include <QtCore/QMetaMethod>


class MetaMethodInvoker {
public:
    using CallContext = std::array<void*, 10>;

    /**
     * Prepares a context to be used with `invoke`.
     *
     * \param method                    Method to call. Must be valid.
     * \param[in,out] args              Arguments to use for the call. This call might change their types.
     * \param[out] ctx                  Call context. Pointers inside the call context will point into variant
     *                                  data inside `args`.
     * \throws MetaCallException
     */
    static void prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx);

    /**
     * Invokes a method given a prepared call context. Arguments must be valid, and the method must actually exist
     * for the provided object.
     *
     * \param object                    Object to invoke the method for.
     * \param method                    Method to invoke.
     * \param ctx                       Previously prepared call context.
     */
    static QVariant invoke(QObject* object, const QMetaMethod& method, const CallContext* ctx);
};
