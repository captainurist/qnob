#include "qnob_args.h"

#include <lib/serialization/enum_serialization.h>

QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(StandardList, Qt::CaseSensitive, {
    { EntitiesList, "entities" },
    { KeysList, "keys" }
})
