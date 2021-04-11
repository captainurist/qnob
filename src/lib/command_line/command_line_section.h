#pragma once

#include <vector>

#include <QtCore/QString>

#include "command_line_option.h"

class CommandLineSection {
public:
    QString name;
    CommandLineOptionList options;
};

using CommandLineSectionList = std::vector<CommandLineSection>;
