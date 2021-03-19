#include "alignment.h"

#include <lib/serialization/enum_serialization.h>

QT_SPECIALIZE_STD_HASH_TO_CALL_QHASH_BY_VALUE(Qt::Alignment)

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
