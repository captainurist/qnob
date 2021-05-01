#include "meta_method_invoker.h"

#include <cassert>

#include "meta_call_exception.h"

void MetaMethodInvoker::prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx) {
    assert(method.isValid());

    int parameterCount = method.parameterCount();
    if (parameterCount != args->size()) {
        xthrow MetaCallException(
            method.enclosingMetaObject(),
            method.name(),
            MetaCallException::tr("Argument count mismatch (%1 != %2).").arg(parameterCount).arg(args->size())
        );
    }

    if (parameterCount >= ctx->size())
        xthrow MetaCallException(method.enclosingMetaObject(), method.name(), MetaCallException::tr("Method has too many arguments."));

    for (int i = 0; i < parameterCount; i++) {
        int expectedTypeId = method.parameterType(i);

        QVariant& arg = (*args)[i];
        if (expectedTypeId != arg.typeId() && !arg.convert(QMetaType(expectedTypeId))) {
            xthrow MetaCallException(
                method.enclosingMetaObject(),
                method.name(),
                MetaCallException::tr("Type mismatch in argument #%1: expected %2, got %3.")
                    .arg(i + 1)
                    .arg(QLatin1String(QMetaType(expectedTypeId).name()))
                    .arg(QLatin1String(arg.typeName()))
            );
        }

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
