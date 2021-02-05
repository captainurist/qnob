#pragma once

#include <array>

#include <QtCore/QVariant>


class MetaMethodInvoker {
public:
    using CallContext = std::array<void*, 10>;

    /**
     * Prepares a context to be used with `invoke`.
     *
     * \param method                    Method to call. Must be valid.
     * \param[in,out] args              Arguments to use for the call. This call might change their types.
     * \param[out] ctx                  Call context. Note that pointers inside the call context will point into variant
     *                                  data inside `args`.
     * \throws MetaCallException
     */
    static void prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx);

    /**
     * Invokes a method given a prepared call context.
     *
     * \param object                    Object to invoke the method for.
     * \param method                    Method to invoke.
     * \param ctx                       Previously prepared call context.
     * \throws MetaCallException
     */
    static QVariant invoke(QObject* object, const QMetaMethod& method, CallContext* ctx);
};
