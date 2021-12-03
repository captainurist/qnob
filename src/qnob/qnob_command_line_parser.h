#pragma once

#include <lib/command_line/command_line_parser.h>

#include "qnob_args.h"

class QnobCommandLineParser : public CommandLineParser {
    using base_type = CommandLineParser;
    Q_DECLARE_TR_FUNCTIONS(QnobCommandLineParser);
public:
    QnobCommandLineParser() {
        QStringList listValues;
        for (int i = 0; i < ListCount; i++)
            listValues.push_back(sformat(L"'{}'", serialized(static_cast<StandardList>(i))));
        listValues.sort();
        QString lists = listValues.join(lit(", "));

        addSection(QString());
        addOption("config", tr("Path to a config file.")).argument("PATH", &m_args.configPath).defaultValue(lit("./qnob.toml"));
        addOption("log", tr("Path to a log file.")).argument("PATH", &m_args.logPath).defaultValue(lit("./qnob.log"));
        addOption("hide-console", tr("Hides console window on startup (Windows only).")).flag(&m_args.hideConsole);
        addOption("list", sformat(tr("Output members of one of the built-in lists. Valid values are {}."), lists)).argument("LIST", &m_args.list);
        addOption("help", tr("Show help and exit.")).flag(&m_help);
        addOption('v', "version", tr("Show version information and exit.")).flag(&m_version);
    }

    QnobArgs parse(const QStringList& commandLine) {
        base_type::parse(commandLine);

        if (m_help) {
            m_args.mode = HelpMode;
        } else if (m_version) {
            m_args.mode = VersionMode;
        } else if (m_args.list != NoList) {
            m_args.mode = ListMode;
        } else {
            m_args.mode = ServiceMode;
        }

        return m_args;
    }

private:
    QnobArgs m_args;
    bool m_help = false;
    bool m_version = false;
};
