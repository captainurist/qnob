#pragma once

#include <functional>
#include <memory>
#include <cassert>

class QPainter;

// TODO: move to gui/ ?
class DeferredPicture {
    using function_type = std::function<void(QPainter*)>;
public:
    DeferredPicture() {}

    template<class Deferred>
    DeferredPicture(Deferred&& deferred):
        m_data(new function_type(std::forward<Deferred>(deferred)))
    {}

    void play(QPainter* painter) {
        if(m_data)
            (*m_data)(painter);
    }

private:
    std::shared_ptr<function_type> m_data;
};
