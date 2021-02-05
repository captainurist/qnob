#include "meta_method_invoker.h"

#include <QtCore/QMetaMethod>

#include <utility/exception.h>

void MetaMethodInvoker::prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx) {
    int parameterCount = method.parameterCount();
    if (parameterCount != args->size())
        qthrow Exception(QString());
    if (parameterCount >= ctx->size())
        qthrow Exception(QString());

    for (int i = 0; i < parameterCount; i++) {
        int expectedTypeId = method.parameterType(i);

        QVariant& arg = (*args)[i];
        if (expectedTypeId != arg.typeId() && !arg.convert(QMetaType(expectedTypeId)))
            qthrow Exception(QString());

        (*ctx)[i + 1] = arg.data();
    }
}

QVariant MetaMethodInvoker::invoke(QObject* object, const QMetaMethod& method, CallContext* ctx) {
    QVariant result;

    int returnTypeId = method.returnType();
    if (method.returnType() != QMetaType::Void) {
        result = QVariant(QMetaType(returnTypeId));
        (*ctx)[0] = result.data();
    }

    object->metaObject()->metacall(object, QMetaObject::InvokeMetaMethod, method.methodIndex(), ctx->data());
    return result;
}
