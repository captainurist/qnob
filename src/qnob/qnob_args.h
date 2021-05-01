#pragma once

#include <QtCore/QString>

#include <lib/serialization/serialization_fwd.h>

enum RunMode {
    ServiceMode,
    HelpMode,
    VersionMode,
    ListMode
};

enum StandardList {
    EntitiesList,
    KeysList,
    ListCount,
    NoList = -1,
};
QB_DECLARE_SERIALIZATION_FUNCTIONS(StandardList)

struct QnobArgs {
    RunMode mode = ServiceMode;
    QString configPath;
    QString logPath;
    bool console = false;
    StandardList list = NoList;
};


