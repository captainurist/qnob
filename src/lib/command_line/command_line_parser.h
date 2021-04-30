#pragma once

#include <vector>

#include <QtCore/QString>
#include <QtCore/QCoreApplication> /* For Q_DECLARE_TR_FUNCTIONS. */

#include "command_line_section.h"
#include "command_line_option_builder.h"
#include "command_line_help_options.h"

class QTextStream;

class CommandLineParser {
    Q_DECLARE_TR_FUNCTIONS(CommandLineParser);
public:
    CommandLineParser() {}

    void addSection(const QString& name) {
        m_sections.emplace_back().name = name;
    }

    CommandLineOptionBuilder addOption(char shortName, const char* longName, const QString& description);

    CommandLineOptionBuilder addOption(const char* longName, const QString& description) {
        return addOption('\0', longName, description);
    }

    CommandLineOptionBuilder addOption(char shortName, const QString& description) {
        return addOption(shortName, nullptr, description);
    }

    void parse(const QStringList& commandLine);

    void printSections(QTextStream& stream, const CommandLineHelpOptions& options = CommandLineHelpOptions()) const;

private:
    CommandLineSectionList m_sections;
};

