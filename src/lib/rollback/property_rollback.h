#pragma once

#include <functional> /* For std::invoke */

template<class T, class Object, auto getter, auto setter>
class PropertyRollback {
public:
    PropertyRollback(Object* object) : m_object(object) {
        m_value = std::invoke(getter, *object);
    }

    PropertyRollback(Object* object, const T& value) : PropertyRollback(object) {
        std::invoke(setter, *object, value);
    }

    void commit() {
        m_commited = true;
    }

    ~PropertyRollback() {
        if (!m_commited)
            std::invoke(setter, *m_object, m_value);
    }

private:
    Object* m_object;
    T m_value;
    bool m_commited = false;
};
