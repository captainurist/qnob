#pragma once

#include <lib/command_line/command_line_parser.h>

#include "qnob_args.h"

class QnobCommandLineParser : public CommandLineParser {
    using base_type = CommandLineParser;
    Q_DECLARE_TR_FUNCTIONS(QnobCommandLineParser);
public:
    QnobCommandLineParser() {
        QStringList listValues;
        for (StandardList list : {EntitiesList})
            listValues.push_back(lit("'%1'").arg(serialize(list)));
        QString lists = listValues.join(lit(", "));

        addSection(QString());
        addOption("config", tr("Path to a config file.")).argument("PATH", &m_args.configPath).defaultValue(lit("./qnob.toml"));
        addOption("log", tr("Path to a log file.")).argument("PATH", &m_args.logPath).defaultValue(lit("./qnob.log"));
        addOption("console", tr("Always opens a console window (Windows only).")).flag(&m_args.console);
        addOption("list", tr("Output members of one of the built-in lists. Valid values are %1.").arg(lists)).argument("LIST", &m_args.list);
        addOption("help", tr("Show help and exit.")).flag(&m_help);
        addOption('v', "version", tr("Show version information and exit.")).flag(&m_version);
    }

    QnobArgs parse(const QStringList& commandLine) {
        base_type::parse(commandLine);

        if (m_help) {
            m_args.mode = HelpMode;
            m_args.console = true;
        } else if (m_version) {
            m_args.mode = VersionMode;
            m_args.console = true;
        } else if (m_args.list != NoList) {
            m_args.mode = ListMode;
            m_args.console = true;
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
