#include "meta_method_invoker.h"

#include <cassert>

#include <util/format.h>

#include "meta_call_exception.h"

void MetaMethodInvoker::prepareInvocation(const QMetaMethod& method, QVariantList* args, CallContext* ctx) {
    assert(method.isValid());

    int parameterCount = method.parameterCount();
    if (parameterCount != args->size()) {
        xthrow MetaCallException(
            method.enclosingMetaObject(),
            method.name(),
            sformat(MetaCallException::tr("Argument count mismatch ({} != {})."), parameterCount, args->size())
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
                sformat(MetaCallException::tr("Type mismatch in argument #{}: expected {}, got {}."),
                    i + 1,
                    QLatin1String(QMetaType(expectedTypeId).name()),
                    QLatin1String(arg.typeName()))
            );
        }

        (*ctx)[i + 1] = arg.data();
    }
}

QVariant MetaMethodInvoker::invoke(QObject* object, const QMetaMethod& method, const CallContext* ctx) {
    assert(object);
    assert(method.isValid());
    assert(ctx);
    assert(object->metaObject()->inherits(method.enclosingMetaObject()));

    QVariant result;
    void** args;
    CallContext tmp; /* std::array doesn't initialize its elements in constructor, so this line is free. */

    int returnTypeId = method.returnType();
    if (returnTypeId == QMetaType::Void) {
        /* No return value => no need for a temporary context. */
        args = const_cast<void**>(ctx->data());
    } else {
        /* We have a return value => need a temporary context to store it properly. */
        result = QVariant(QMetaType(returnTypeId));
        tmp = *ctx;
        tmp[0] = result.data();
        args = tmp.data();
    }

    object->metaObject()->metacall(object, QMetaObject::InvokeMetaMethod, method.methodIndex(), args);
    return result;
}
