#include "skin_type.h"

#include <lib/serialization/enum_serialization.h>

X_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(SkinType, Qt::CaseSensitive, {
    { HorizontalBarSkinType, "horizontal_bar_skin" },
    { NumericSkinType, "numeric_skin" }
})
