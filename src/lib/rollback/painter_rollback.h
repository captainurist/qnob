#pragma once

#include <QtGui/QPainter>

#include "property_rollback.h"

namespace detail {

inline void setPainterTransform(QPainter& painter, const QTransform& transform) {
    painter.setTransform(transform, false);
}

}

using PainterPenRollback = PropertyRollback<QPen, QPainter, &QPainter::pen, static_cast<void(QPainter::*)(const QPen&)>(&QPainter::setPen)>;
using PainterFontRollback = PropertyRollback<QFont, QPainter, &QPainter::font, &QPainter::setFont>;
using PainterTransformRollback = PropertyRollback<QTransform, QPainter, &QPainter::transform, &detail::setPainterTransform>;
