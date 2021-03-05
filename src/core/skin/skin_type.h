#pragma once

#include <lib/serialization/serialization_fwd.h>

enum SkinType {
    HorizontalBarSkinType,
    NumericSkinType
};

QB_DECLARE_SERIALIZATION_FUNCTIONS(SkinType)
