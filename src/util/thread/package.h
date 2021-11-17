#pragma once

#include <cassert>

#include <memory>

/**
 * Helper class to pass move-only types in Qt signals / slots.
 */
template<class T>
class Package {
public:
    Package(T&& payload) {
        put(std::move(payload));
    }

    bool empty() {
        return !m_payload;
    }

    T take() {
        assert(!empty());

        T result = std::move(*m_payload);
        m_payload.reset();
        return result;
    }

    void put(T&& payload) {
        m_payload = std::make_shared<T>(std::move(payload));
    }

private:
    std::shared_ptr<T> m_payload;
};
