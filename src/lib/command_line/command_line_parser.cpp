#include "command_line_parser.h"

#include <cassert>

#include <QtCore/QStringList>
#include <QtCore/QCommandLineParser>
#include <QtCore/QTextStream>

#include <util/format.h>

#include "command_line_exception.h"

static QString optionName(const CommandLineOption& option) {
    if (!option.longName.isEmpty()) {
        return option.longName;
    } else {
        assert(!option.shortName.isNull());
        return option.shortName;
    }
}

static QCommandLineOption toBaseOption(const CommandLineOption& option) {
    QStringList names;
    if (!option.shortName.isNull())
        names.push_back(option.shortName);
    if (!option.longName.isEmpty())
        names.push_back(option.longName);

    QCommandLineOption result(names);
    result.setDescription(option.description);
    result.setValueName(option.valueName);
    return result;
}

static QString whitespace(size_t size) {
    return QString(size, QLatin1Char(' '));
}

static QStringList wrapText(const QString& text, size_t size) {
    QStringList result;

    size_t breakablePos = 0;
    size_t lineLength = 0;
    size_t lineStart = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        lineLength++;

        QChar c = text.at(i);
        if (c.isSpace())
            breakablePos = i;

        size_t breakPos = 0;
        size_t startPos = 0;
        if (lineLength > size && breakablePos != 0) {
            /* OK to break! */
            breakPos = breakablePos;
            startPos = breakablePos + 1;
        } else if (lineLength > size && breakablePos == 0) {
            /* Nowhere to break, so break mid-word. */
            breakPos = i;
            startPos = i;
        } else if (i == text.size() - 1) {
            /* We don't want weird ifs after the loop. */
            breakPos = i + 1;
            startPos = i + 1;
        } else if (c == QLatin1Char('\n')) {
            /* Forced break. */
            breakPos = i;
            startPos = i + 1;
        }

        if (breakPos != 0) {
            result.push_back(text.mid(lineStart, breakPos - lineStart));
            breakablePos = 0;
            lineLength = i - startPos + 1;
            lineStart = startPos;
        }
    }

    return result;
}

CommandLineOptionBuilder CommandLineParser::addOption(char shortName, const char* longName, const QString& description) {
    assert(shortName != '\0' || longName != nullptr); /* Should have a name! */
    assert(!m_sections.empty());

    CommandLineOption option;
    option.shortName = QLatin1Char(shortName);
    option.longName = QLatin1String(longName);
    option.description = description;
    return &m_sections.back().options.emplace_back(option);
}

void CommandLineParser::parse(const QStringList& commandLine) {
    QCommandLineParser parser;
    for (const CommandLineSection& section : m_sections) {
        for (const CommandLineOption& option : section.options) {
            bool success = parser.addOption(toBaseOption(option));
            assert(success);
        }
    }

    if (!parser.parse(commandLine))
        xthrow CommandLineException(parser.errorText());

    for (const CommandLineSection& section : m_sections) {
        for (const CommandLineOption& option : section.options) {
            QString name = optionName(option);

            if (option.required && !parser.isSet(name))
                xthrow CommandLineException(sformat(CommandLineException::tr("Required option '{}' is not provided."), name));

            QStringList values = parser.values(name);
            if (values.isEmpty() && !option.defaultValue.isEmpty())
                values.push_back(option.defaultValue);
            if (values.isEmpty() && option.valueName.isEmpty() && parser.isSet(name))
                values.push_back(QString()); /* Flag option. */

            for (const QString& value : values) {
                try {
                    option.handler(value);
                } catch (...) {
                    xthrow CommandLineException(sformat(CommandLineException::tr("Could not parse value for option '{}'."), name));
                }
            }
        }
    }
}

void CommandLineParser::printSections(QTextStream& stream, const CommandLineHelpOptions& options) const {
    bool hasShortOptions;
    for (const CommandLineSection& section : m_sections)
        for (const CommandLineOption& option : section.options)
            if (!option.hidden)
                hasShortOptions |= !option.shortName.isNull();

    auto formatHeader = [&](const CommandLineOption& option) {
        QString result;

        if (hasShortOptions) {
            if (option.shortName.isNull()) {
                result += whitespace(4);
            } else {
                result += QLatin1Char('-');
                result += option.shortName;
                if (!option.longName.isEmpty()) {
                    result += lit(", ");
                } else if (!option.valueName.isEmpty()) {
                    result += QLatin1Char(' ');
                }
            }
        }

        if (!option.longName.isEmpty()) {
            result += lit("--");
            result += option.longName;
            if (!option.valueName.isEmpty())
                result += QLatin1Char('=');
        }

        if (!option.valueName.isEmpty())
            result += option.valueName;

        return result;
    };

    size_t headerLength = 0;
    for (const CommandLineSection& section : m_sections)
        for (const CommandLineOption& option : section.options)
            if (!option.hidden)
                headerLength = std::max(headerLength, static_cast<size_t>(formatHeader(option).size()));
    headerLength = std::min(headerLength, options.maxHeaderLength);

    for (const CommandLineSection& section : m_sections) {
        if (!section.name.isEmpty())
            stream << section.name << ":" << Qt::endl;

        for (const CommandLineOption& option : section.options) {
            if(option.hidden)
                continue;

            QString header = formatHeader(option);
            QString description = option.description;
            if (options.printDefaults && !option.defaultValue.isEmpty()) {
                description += description.endsWith(QLatin1Char('.')) ? lit(" ") : lit(". ");
                description += sformat(tr("Default is '{}'."), option.defaultValue);
            }
            QStringList lines = wrapText(description, options.maxLineLength - options.indent - headerLength - options.spacing);
            if (lines.isEmpty())
                lines.push_back(QString()); /* Empty description, shouldn't really happen, but we still don't want to crash. */

            stream << whitespace(options.indent) << header;

            if (header.size() > headerLength) {
                stream << Qt::endl;
                stream << whitespace(options.indent + headerLength);
            } else {
                stream << whitespace(headerLength - header.size());
            }

            stream << whitespace(options.spacing) << lines[0] << Qt::endl;
            for (size_t i = 1; i < lines.size(); i++)
                stream << whitespace(options.indent + headerLength + options.spacing) << lines[i] << Qt::endl;
        }

        stream << Qt::endl;
    }
}
