#pragma once

#include <vector>
#include <functional>

#include <QtCore/QCommandLineOption>

class CommandLineOption {
public:
    /** Short option name. */
    QChar shortName;

    /** Long option name. */
    QString longName;

    /** Option description. */
    QString description;

    /** Whether the option is required. */
    bool required = false;

    /** Whether the option is hidden. */
    bool hidden = false;

    /** Name of the option's value. If set, value is required. */
    QString valueName;

    /** Default value to be passed into handler if no value was specified on the command line. */
    QString defaultValue;

    /** Option handler. */
    std::function<void(QStringView)> handler;
};

using CommandLineOptionList = std::vector<CommandLineOption>;
