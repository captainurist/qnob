#pragma once

#include <array>

#include <QtCore/QVariant>


class MetaMethodInvoker {
public:
    using CallContext = std::array<void*, 10>;

    static void prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx);
    static QVariant invoke(QObject* object, const QMetaMethod& method, CallContext* ctx);

};
