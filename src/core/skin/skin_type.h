#pragma once

#include <lib/serialization/serialization_fwd.h>

enum SkinType {
    HorizontalBarSkinType,
    NumericSkinType
};

X_DECLARE_SERIALIZATION_FUNCTIONS(SkinType)
