#pragma once

#include <functional>

#include <core/knob/knob_state.h>

class QPainter;

class KnobPainter {
public:
    template<class PaintFunction>
    KnobPainter(PaintFunction&& paintFunction):
        m_function(std::forward<PaintFunction>(paintFunction))
    {}

    /**
     * Paints the knob state using the provided painter at (0, 0).
     *
     * \param painter                   Painter to use.
     * \param state                     Knob state to paint.
     */
    void paint(QPainter* painter, const KnobState& state) {
        m_function(painter, state);
    }

private:
    std::function<void(QPainter*, const KnobState&)> m_function;
};

