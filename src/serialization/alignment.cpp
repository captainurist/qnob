#include "alignment.h"

#include "enum_serialization.h"

QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(Qt::Alignment, ({
    {Qt::AlignTop | Qt::AlignLeft, "top_left"},
    {Qt::AlignTop | Qt::AlignHCenter, "top"},
    {Qt::AlignTop | Qt::AlignRight, "top_right"},
    {Qt::AlignVCenter | Qt::AlignLeft, "left"},
    {Qt::AlignVCenter | Qt::AlignHCenter, "center"},
    {Qt::AlignVCenter | Qt::AlignRight, "right"},
    {Qt::AlignBottom | Qt::AlignLeft, "bottom_left"},
    {Qt::AlignBottom | Qt::AlignHCenter, "bottom"},
    {Qt::AlignBottom | Qt::AlignRight, "bottom_right"}
}))
