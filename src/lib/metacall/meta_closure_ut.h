#pragma once

#include <vector>

#include <QtCore/QObject>

/* This workaround is needed here because moc refuses to handle reference return types.
 * If a method is declared as returning a reference, it will be exposed as returning void in the metaobject.
 * The reason is probably pretty straightforward - you cannot have a pointer to a reference, and the way return type
 * is handled in the metaobject is with an output void* pointer.
 *
 * A better workaround might be warranted here, e.g. returning Ref<T>, and handling this properly in the next layer. */
#ifdef Q_MOC_RUN
#   define X_REF(... /* TYPE */) __VA_ARGS__
#   define X_CREF(... /* TYPE */) __VA_ARGS__
#else
#   define X_REF(... /* TYPE */) __VA_ARGS__&
#   define X_CREF(... /* TYPE */) const __VA_ARGS__&
#endif

class MetaClosureTest : public QObject {
    Q_OBJECT
public:
    MetaClosureTest(QObject* parent) : QObject(parent) {}

    Q_INVOKABLE void setValue(int value) {
        m_value = value;
    }

    Q_INVOKABLE int value() const {
        return m_value;
    }

    Q_INVOKABLE void setData(const std::vector<int>& data) {
        m_data = data;
    }

    Q_INVOKABLE X_CREF(std::vector<int>) data() const {
        return m_data;
    }

private:
    int m_value = 0;
    std::vector<int> m_data;
};
